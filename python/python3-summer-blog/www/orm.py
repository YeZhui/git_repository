# -*-coding:utf-8 -*-
import asyncio,log,logging
import aiomysql

log.logConfig('error.log')

@asyncio.coroutine #标记为一个协程
def create_pool(loop, **kw):
	logging.debug('create database connection pool...')
	#定义全局变量
	global __pool
	__pool = yield from aiomysql.create_pool(
		host = kw.get('host', 'localhost'),
		port = kw.get('port', 3306),
		user = kw['user'],
		password = kw['password'],
		db = kw['db'],
		charset = kw.get('charset', 'utf8'),
		autocommit = kw.get('autocommit', True),
		maxsize = kw.get('maxsize', 10),
		minsize = kw.get('minsize', 1),
		loop =loop)

@asyncio.coroutine
def destory_pool():
	global __pool
	if __pool is not None:
		__pool.close()
		yield from __pool.wait_closed()

@asyncio.coroutine
def select(sql, args, size=None):
	logging.debug(sql)
	global __pool
	with (yield from __pool) as conn:
		cur = yield from conn.cursor(aiomysql.DictCursor)
		yield from cur.execute(sql.replace('?', '%s'), args or ())
		if size:
			rs = yield from cur.fetchmany(size)
		else:
			rs = yield from cur.fetchall()
		yield from cur.close()
		logging.info('rows returned; %s' % len(rs))
		return rs
		
@asyncio.coroutine
def selectFormat(sql, size=None):
	logging.debug(sql)
	global __pool
	with (yield from __pool) as conn:
		cur = yield from conn.cursor(aiomysql.DictCursor)
		yield from cur.execute(sql)
		if size:
			rs = yield from cur.fetchmany(size)
		else:
			rs = yield from cur.fetchall()
		yield from cur.close()
		logging.info('rows returned; %s' % len(rs))
		return rs

#insert,update,delete
@asyncio.coroutine
def execute(sql, args):
	logging.debug(sql)
	with (yield from __pool) as conn:
		try:
			cur = yield from conn.cursor()
			yield from cur.execute(sql.replace('?', '%s'), args)
			affected = cur.rowcount #返回结果数
			yield from cur.close()
		except BaseException as e:
			raise
		return affected

def create_args_string(num):
	L = []
	for n in range(num):
		L.append('?')
	return (','.join(L))

#Model类
class ModelMetaclass(type):
	def __new__(cls, name, bases, attrs):
		if name == 'Model':
			return type.__new__(cls, name, bases, attrs)
		tableName = attrs.get('__table__', None) or name
		logging.info('found model: %s (table: %s)' % (name, tableName))
		#获取所有的Field和主键名
		mappings = dict()
		fields = []
		primaryKey = None
		for k, v in attrs.items():
			if isinstance(v, Field):
				logging.info(' found mapping:%s ==> %s' % (k,v))
				mappings[k] = v
				if v.primary_key:
					if primaryKey:
						raise RuntimeError('Duplicate primary key for field:%s' % k)
					primaryKey = k
				else:
					fields.append(k)

		if not primaryKey:
			raise RuntimeError('Primary key not found.')
		for k in mappings.keys():
			attrs.pop(k)
		escaped_fields = list(map(lambda f:'%s' % f, fields))
		attrs['__mappings__'] = mappings
		attrs['__table__'] = tableName
		attrs['__primary_key__'] = primaryKey
		attrs['__fields__'] = fields
		attrs['__select__'] = 'select `%s`, %s from `%s`' % (primaryKey, ', '.join(escaped_fields), tableName)
		attrs['__insert__'] = 'insert into `%s` (%s, `%s`) values (%s)' % (tableName, ', '.join(escaped_fields), primaryKey, create_args_string(len(escaped_fields) + 1))
		attrs['__update__'] = 'update `%s` set %s where `%s`=?' % (tableName, ', '.join(map(lambda f: '`%s`=?' % (mappings.get(f).name or f), fields)), primaryKey)
		attrs['__delete__'] = 'delete from `%s` where `%s`=?' % (tableName, primaryKey)
		return type.__new__(cls, name, bases, attrs)

class Model(dict, metaclass=ModelMetaclass):
	def __init__(self, **kw):
		super(Model, self).__init__(**kw)

	def __getattr__(self, key):
		try:
			return self[key]
		except KeyError:
			raise AttributeError(r"'Model' object has no attribute'%s'" % key)

	def __setattr(self,key,value):
		self[key] = value

	def getValue(self,key):
		return getattr(self,key,None)

	def getValueOrDefault(self,key):
		value = getattr(self, key, None)
		if value is None:
			field = self.__mappings__[key]
			if field.default is not None:
				value = field.default() if callable(field.default) else field.default
				logging.debug('using default value for %s: %s' % (key, str(value)))
				setattr(self, key, value)
		return value

	@classmethod
	@asyncio.coroutine
	def find(cls, pk):
		'find object my primary key.'
		rs = yield from select('%s where %s=?' % (cls.__select__, cls.__primary_key__), pk, 1)
		if len(rs) == 0:
			return None
		return cls(**rs[0])
		
	@classmethod
	@asyncio.coroutine
	def findByField(cls, field, value, select=None):
		'find object by any field.'
		if select is None:
			rs = yield from selectFormat("%s where %s='%s'" % (cls.__select__, field, value))
		else:
			rs = yield from selectFormat("%s where %s='%s'" % (select, field, value))
		if len(rs) == 0:
			return None
		return [cls(**r) for r in rs]

	@asyncio.coroutine
	def save(self):
		args = list(map(self.getValueOrDefault, self.__fields__))
		args.append(self.getValueOrDefault(self.__primary_key__))
		rows = yield from execute(self.__insert__, args)
		if rows != 1:
			logging.error('failed to insert record:affected rows: %s' % rows)

	@asyncio.coroutine
	def update(self):
		args = list(map(self.getValue, self.__fields__))
		args.append(self.getValue(self.__primary_key__))
		logging.debug(args)
		rows = yield from execute(self.__update__, args)
		if rows != 1:
			logging.error('failed to update field:%s, rows:%s' % (id,rows))
	
	@asyncio.coroutine
	def remove(self):
		args = [self.getValue(self.__primary_key__)]
		rows = yield from execute(self.__delete__, args)
		if rows != 1:
			logging.error('failed to delete by primary key')

	@classmethod
	@asyncio.coroutine
	def findAll(cls, where=None, args=None, **kw):
		sql = [cls.__select__]
		if where:
			sql.append('where')
			sql.append(where)
		if args is None:
			args = []
		orderBy = kw.get('orderBy', None)
		if orderBy:
			sql.append('order by')
			sql.append(orderBy)
		limit = kw.get('limit', None)
		if limit is not None:
			sql.append('limit')
			if isinstance(limit, int):
				sql.append('?')
				args.append(limit)
			elif isinstance(limit, tuple) and len(limit) == 2:
				sql.append('?,?')
				args.extend(limit)
			else:
				raise ValueError('Invalid limit value:%s' % str(limit))
		rs = yield from select(' '.join(sql), args)
		return [cls(**r) for r in rs]

	@classmethod
	@asyncio.coroutine
	def findNumber(cls, selectField, where=None, args=None):
		sql = ['select %s _num_ from `%s`' % (selectField, cls.__table__)]
		if where:
			sql.append('where')
			sql.append(where)
		rs = yield from select(' '.join(sql), args, 1)
		if len(rs) == 0:
			return None
		return rs[0]['_num_']

class Field(object):
	def __init__(self, name, column_type, primary_key, default):
		self.name = name
		self.column_type = column_type
		self.primary_key = primary_key
		self.default = default

	def __str__(self):
		return '<%s, %s:%s>' % (self.__class__.__name__, self.column_type, self.name)

class StringField(Field):
	def __init__(self, name=None, primary_key = False, default=None, column_type='varchar(100)'):
		super().__init__(name, column_type, primary_key, default)

class IntegerField(Field):
	def __init__(self, name=None, primary_key = False, default=None, column_type='int(4)'):
		super().__init__(name, column_type, primary_key, default)

class BooleanField(Field):
	def __init__(self, name=None, primary_key = False, default=0, column_type='bool'):
		super().__init__(name, column_type, primary_key, default)

class DecimalField(Field):
	def __init__(self, name=None, primary_key = False, default=None, column_type='float'):
		super().__init__(name, column_type, primary_key, default)

class TextField(Field):
	def __init__(self, name=None, primary_key = False, default=None, column_type='text'):
		super().__init__(name, column_type, primary_key, default)

