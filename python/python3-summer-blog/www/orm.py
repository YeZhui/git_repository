# -*-coding:utf-8 -*-
import asyncio,log
import aiomysql

logConfig('error.log')

@asyncio.coroutine
def create_pool(loop, **kw):
	logOutput('create database connection pool...')
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
def select(sql, args, size=None):
	logOutput(sql,args)	
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

#insert,update,delete
@asyncio.coroutine
def execute(sql, args):
	logOutput(sql,args)
	with (yield from __pool) as conn:
		try:
			cur = yield from conn.cursor()
			yield from cur.execute(sql.replace('?', '%s'), args)
			affected = cur.rowcount #返回结果数
			yield from cur.close()
		except BaseException as e:
			raise
		return affected

#Model类
class Model(dict, metaclass=ModelMetaclass):
	def __init__(self, **kw):
		super(Model, self).__init__(**kw)

	def __getattr
