# -*-coding:utf-8 -*-
import time, uuid
from orm import Model, StringField, DecimalField, TextField, IntegerField, BooleanField
import orm,aiomysql,asyncio
import sys,log

def next_id():
	return '%015d%s000' % (int(time.time() * 1000), uuid.uuid4().hex)

class User(Model):
	__table__ = 'users'
	id = StringField(primary_key=True, default=next_id, column_type='varchar(50)')
	email = StringField(column_type='varchar(50)')
	passwd = StringField(column_type='varchar(50)')
	admin = BooleanField()
	name = StringField(column_type='varchar(50)')
	image = StringField(column_type='varchar(50)')
	created_at = DecimalField(default=time.time, column_type='float')

class Blog(Model):
	__table__ = 'blogs'
	id = StringField(primary_key=True, default=next_id, column_type='varchar(50)')
	user_id = StringField(column_type='varchar(50)')
	user_name = StringField(column_type='varchar(50)')
	user_image = StringField(column_type='varchar(500)')
	name = StringField(column_type='varchar(50)')
	summary = StringField(column_type='varchar(200)')
	content = TextField()
	created_at = DecimalField(default=time.time)

class Comment(Model):
	__table__ = 'comments'
	id = StringField(primary_key=True, default=next_id, column_type='varchar(50)')
	blog_id = StringField(column_type='varchar(50)')
	user_id = StringField(column_type='varchar(50)')
	user_name = StringField(column_type='varchar(50)')
	user_image = StringField(column_type='varchar(500)')
	content = TextField()
	created_at = DecimalField(default=time.time)

@asyncio.coroutine
def test(loop):
	yield from orm.create_pool(loop, host = 'kingdom', user='mysql', password='mysql', db='summer')
	#u = User(name='Test', email='test@example.com', passwd='123456', image='about:blank', admin=1)
	#yield from u.save()
	u = User(name='Test', email='guanyz@example.com', passwd='123456', image='about:blank', admin=1, created_at=1490001869.61995, id='0014900018696207ff3cf170ee84a0e945df4d1e84d12eb000')
	#u = User(name='Test', passwd='123456', email='guanyz@szkingdom.com', id='0014900018696207ff3cf170ee84a0e945df4d1e84d12eb000')
	yield from u.update()
	r = yield from User().find('0014900018696207ff3cf170ee84a0e945df4d1e84d12eb000')
	print(r)

if __name__ == '__main__':
	loop = asyncio.get_event_loop()
	loop.run_until_complete(test(loop))
	loop.close()
#loop.run_forever()
	if loop.is_closed():
		sys.exit(0) #0表示正常退出
