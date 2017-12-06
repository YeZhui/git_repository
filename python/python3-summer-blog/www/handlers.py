# -*- coding: utf-8 -*-
__author__ = 'YeZhui Guan'

' url handlers '

import re,time,json, logging, hashlib, base64, asyncio
import markdown2
import time

from coroweb import get,post
from models import User,Comment, Blog, next_id, Classification, Draft

from apis import APIValueError, APIResourceNotFoundError, APIError, Page, APIPermissionError
from config import configs
from aiohttp import web
from datetime import datetime

import log
log.logConfig(log.runLog)

COOKIE_NAME = 'sumsession'
_COOKIE_KEY = configs.session.secret

def check_admin(request):
	if request.__user__ is None or not request.__user__.admin:
		raise APIPermissionError()
		
def datetime_convert(t):
	dt = datetime.fromtimestamp(t)
	return int('%04d%02d' % (dt.year, dt.month))
	
def get_datetime(t):
	dt = datetime.fromtimestamp(t)
	return str('%04d年%02d月%02d日 %02d:%02d:%02d' % (dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second))

def get_page_index(page_str):
	p = 1
	try:
		p = int(page_str)
	except ValueError as e:
		pass
	if p < 1:
		p = 1
	return p

def user2cookie(user,max_age):
	expires = str(int(time.time() + max_age))
	s= '%s-%s-%s-%s' % (user.id, user.passwd, expires, _COOKIE_KEY)
	L = [user.id, expires, hashlib.sha1(s.encode('utf-8')).hexdigest()]
	return '-'.join(L)

def text2html(text):
	lines = map(lambda s: '<p>%s</p>' % s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;'), filter(lambda s: s.strip() != '', text.split('\n')))
	return ''.join(lines)
	
@asyncio.coroutine
def cookie2user(cookie_str):
	if not cookie_str:
		return None
	try:
		L = cookie_str.split('-')
		if len(L) != 3:
			return None
		uid, expires, sha1 = L
		if int(expires) < time.time():
			return None
		user = yield from User.find(uid)
		if user is None:
			return None
		s = '%s-%s-%s-%s' % (uid, user.passwd, expires, _COOKIE_KEY)
		if sha1 != hashlib.sha1(s.encode('utf-8')).hexdigest():
			logging.info('invalid sha1')
			return None
		user.passwd = '******'
		return user
	except Exception as e:
		logging.exception(e)
		return None

@get('/index/{page}')
def index_page(request, *, page = '1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)')
	page = Page(num,page_index)
	if num == 0:
		blogs = []
		date_blogs = []
	else:
		blogs = yield from Blog.findAll(orderBy = 'created_at desc', limit = (page.offset, page.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(date_blog.blog_date)
	date_blogs = list(s)
	
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
		
	pages = range(1,page.page_count+1)
	page_flag = 0
	
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : page,
		'pages' : pages,
		'page_flag' : page_flag,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/')
def index(request, *, page = '1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)')
	page = Page(num,page_index)
	if num == 0:
		blogs = []
		date_blogs = []
	else:
		blogs = yield from Blog.findAll(orderBy = 'created_at desc', limit = (page.offset, page.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(date_blog.blog_date)
	date_blogs = list(s)
	
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
		
	pages = range(1,page.page_count+1)
	page_flag = 0
	
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : page,
		'pages' : pages,
		'page_flag' : page_flag,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/blogs/classes/{classes_id}')
def api_get_blog_by_classes_id(request, *, classes_id, page='1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)', where="classes_id='%s'" % classes_id)
	p = Page(num, page_index)
	if num == 0:
		blogs = []
		date_blogs = []
	else:
		blogs = yield from Blog.findAll(where="classes_id='%s'" % classes_id, limit = (p.offset, p.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(datetime_convert(date_blog.created_at))
	date_blogs = list(s)
	pages = range(1,p.page_count+1)
	page_flag = 1
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : p,
		'pages' : pages,
		'page_flag' : page_flag,
		'classes_id' : classes_id,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/blogs/classes/{classes_id}/{page}')
def api_get_blog_by_classes_id_page(request, *, classes_id, page='1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)', where="classes_id='%s'" % classes_id)
	p = Page(num, page_index)
	if num == 0:
		blogs = []
		date_blogs = []
	else:
		blogs = yield from Blog.findAll(where="classes_id='%s'" % classes_id, limit = (p.offset, p.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(datetime_convert(date_blog.created_at))
	date_blogs = list(s)
	pages = range(1,p.page_count+1)
	page_flag = 1
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : p,
		'pages' : pages,
		'page_flag' : page_flag,
		'classes_id' : classes_id,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/blogs/date/{blog_date}')
def api_get_blog_by_date(request, *, blog_date, page='1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)', where="blog_date='%s'" % blog_date)
	p = Page(num, page_index)
	if num == 0:
		blogs = []
	else:
		blogs = yield from Blog.findAll(where="blog_date='%s'" % blog_date, limit = (p.offset, p.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(datetime_convert(date_blog.created_at))
	date_blogs = list(s)
	pages = range(1,p.page_count+1)
	page_flag = 2
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : p,
		'pages' : pages,
		'page_flag' : page_flag,
		'blog_date' : blog_date,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/blogs/date/{blog_date}/{page}')
def api_get_blog_by_date_page(request, *, blog_date, page='1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)', where="blog_date='%s'" % blog_date)
	p = Page(num, page_index)
	if num == 0:
		blogs = []
	else:
		blogs = yield from Blog.findAll(where="blog_date='%s'" % blog_date, limit = (p.offset, p.limit))
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(datetime_convert(date_blog.created_at))
	date_blogs = list(s)
	pages = range(1,p.page_count+1)
	page_flag = 2
	return {
		'__template__': 'blogs.html',
		'blogs' : blogs,
		'page' : p,
		'pages' : pages,
		'page_flag' : page_flag,
		'blog_date' : blog_date,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__': request.__user__
	}
	
@get('/about')
def about(request):
	return {
		'__template__':'about.html',
		'__user__': request.__user__
	}

@get('/blog/{id}')
def get_blog(id, request):
	blog = yield from Blog.find(id)
	comments = yield from Comment.findAll('blog_id=?', [id], orderBy = 'created_at desc')
	for c in comments:
		c.html_content = text2html(c.content)
	blog.html_content = markdown2.markdown(blog.content, extras=["footnotes","cuddled-lists", "fenced-code-blocks"])
	
	num = yield from Blog.findNumber('count(id)')
	date_blogs = []
	if num > 0:
		date_blogs = yield from Blog.findAll(orderBy = 'created_at desc')
	s = set()
	for date_blog in date_blogs:
		s.add(date_blog.blog_date)
	date_blogs = list(s)
	
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
	
	return {
		'__template__':'blog.html',
		'blog':blog,
		'comments':comments,
		'classes' : classes,
		'date_blogs' : date_blogs,
		'__user__':request.__user__
	}

@get('/register')
def register():
	return {
		'__template__': 'register.html'
	}

@get('/signin')
def signin():
	return {
		'__template__': 'signin.html',
	}
	
@post('/api/save/passwd')
def save_passwd(request, *, passwd1, passwd2):
	if not passwd1:
		raise APIValueError('passwd1', 'Invalid passwd1.')
	if not passwd2:
		raise APIValueError('passwd2', 'Invalid passwd2.')
		
	#验证旧密码
	user = yield from User.find(request.__user__.id)
	sha1 = hashlib.sha1()
	sha1.update(user.id.encode('utf-8'))
	sha1.update(b':')
	sha1.update(passwd1.encode('utf-8'))
	if user.passwd != sha1.hexdigest():
		raise APIValueError('passwd', 'Invalid password.')
		
	#保存新密码
	sha1_passwd = '%s:%s' % (user.id, passwd2)
	user.passwd = hashlib.sha1(sha1_passwd.encode('utf-8')).hexdigest()
	yield from user.update()
	r = web.Response()
	r.set_cookie(COOKIE_NAME, user2cookie(user, 86400), max_age=86400, httponly=True)
	user.passwd = '******'
	r.content_type = 'application/json'
	r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
	return r

@post('/api/authenticate')
def authenticate(*, email, passwd):
	if not email:
		raise APIValueError('email', 'Invalid email.')
	if not passwd:
		raise APIValueError('passwd', 'Invalid password.')
	users = yield from User.findAll('email=?', [email])
	if len(users) == 0:
		raise APIValueError('email', 'Email not exist.')
	user = users[0]
	sha1 = hashlib.sha1()
	sha1.update(user.id.encode('utf-8'))
	sha1.update(b':')
	sha1.update(passwd.encode('utf-8'))
	if user.passwd != sha1.hexdigest():
		raise APIValueError('passwd', 'Invalid password.')
	r = web.Response()
	r.set_cookie(COOKIE_NAME, user2cookie(user, 86400), max_age=86400, httponly=True)
	user.passwd = '******'
	r.content_type = 'application/json'
	r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
	return r

@get('/signout')
def signout(request):
	referer = request.headers.get('Referer')
	r = web.HTTPFound(referer)
	r.set_cookie(COOKIE_NAME, '-deleted-', max_age=0, httponly=True)
	logging.info('user signed out.')
	return r
	
@get('/manage/')
def manage():
	return 'redirect:/manage/comments'
	
@get('/manage/users')
def manage_users(request, *, page='1'):
	return {
		'__template__':'manage_users.html',
		'page_index':get_page_index(page),
		'__user__': request.__user__
	}
	
@get('/manage/comments')
def manage_comments(request, *, page = '1'):
	return {
		'__template__':'manage_comments.html',
		'page_index':get_page_index(page),
		'__user__': request.__user__
	}

@get('/manage/blogs')
def manage_blogs(request, *, page='1'):
	return {
		'__template__':'manage_blog.html',
		'page_index':get_page_index(page),
		'__user__': request.__user__
	}
	
@get('/manage/classes')
def manage_classes(request, *, page='1'):
	return {
		'__template__':'manage_classes.html',
		'page_index':get_page_index(page),
		'__user__': request.__user__
	}
	
@get('/manage/blogs/create')
def manage_create_blog(request ):
	drafts = []
	num = yield from Classification.findNumber('count(id)')
	if num == 0:
		classes = []
		cur_class = '' 
		blogs = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
		cur_class = classes[0].id  #分类id是唯一的
		blogs = yield from Blog.findAll(where="classes_id='%s'" % cur_class)
		draftNum = yield from Draft.findNumber('count(id)', where="classes_id='%s'" % cur_class)
		if draftNum == 1:
			drafts = yield from Draft.findAll(where="classes_id='%s'" % cur_class)
		
	return {
		'__template__':'create_blog.html',
		'id':'',
		'classes':classes,
		'cur_class':cur_class,
		'blogs':blogs,
		'drafts':drafts,
		'isDraft': 0,
		'action':'/api/blogs',
		'__user__': request.__user__
	}
	
@get('/manage/blogs/create/{classes_id}')
def manage_create_blog_class(classes_id, request , * ,isDraft = 0):
	isDraft = isDraft;
	num = yield from Classification.findNumber('count(id)')
	drafts = []
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
		blogs = yield from Blog.findAll(where="classes_id='%s'" % classes_id)
		draftNum = yield from Draft.findNumber('count(id)', where="classes_id='%s'" % classes_id)
		if draftNum == 1:
			drafts = yield from Draft.findAll(where="classes_id='%s'" % classes_id)
		
	return {
		'__template__':'create_blog.html',
		'id':'',
		'classes':classes,
		'cur_class':classes_id,
		'blogs':blogs,
		'drafts':drafts,
		'isDraft': isDraft,
		'action':'/api/blogs',
		'__user__': request.__user__
	}
	
@get('/manage/blogs/edit/{classes_id}')
def manage_edit_blog(classes_id, request, *, id):
	isDraft = 0;
	num = yield from Classification.findNumber('count(id)')
	drafts = []
	if num == 0:
		classes = []
	else:
		classes = yield from Classification.findAll(orderBy = 'created_at desc')
		blogs = yield from Blog.findAll(where="classes_id='%s'" % classes_id)
		draftNum = yield from Draft.findNumber('count(id)', where="classes_id='%s'" % classes_id)
		if draftNum == 1:
			drafts = yield from Draft.findAll(where="classes_id='%s'" % classes_id)
	return {
		'__template__':'create_blog.html',
		'id':id,
		'classes':classes,
		'cur_class':classes_id,
		'blogs':blogs,
		'drafts':drafts,
		'isDraft': isDraft,
		'action':'/api/blogs',
		'__user__': request.__user__
	}
	
@get('/manage/classes/create')
def manage_create_classes(request):
	return {
		'__template__':'manage_classes_edit.html',
		'id':'',
		'action':'/api/classes',
		'__user__': request.__user__
	}
	
@get('/manage/classes/edit')
def manage_edit_classes(request, *, id):
	return {
		'__template__':'manage_classes_edit.html',
		'id':id,
		'action':'/api/classes/%s' % id,
		'__user__': request.__user__
	}
	
@get('/api/comments')
def api_comments(request, *, page='1'):
	page_index = get_page_index(page)
	num = yield from Comment.findNumber('count(id)')
	p = Page(num, page_index)
	if num == 0:
		return dict(page=p, comments=())
	comments = yield from Comment.findAll(orderBy='created_at desc', limit=(p.offset, p.limit))
	return dict(page=p, comments=comments)
	
@get('/api/classes')
def api_classes(request, *, page='1'):
	page_index = get_page_index(page)
	num = yield from Classification.findNumber('count(id)')
	p = Page(num, page_index)
	if num == 0:
		return dict(page=p, classes=())
	classes = yield from Classification.findAll(orderBy='created_at desc', limit = (p.offset, p.limit))
	return dict(page=p, classes = classes)
	
@post('/api/blogs/{id}/comments')
def api_create_comment(id, request, *, content):
	user = request.__user__
	if user is None:
		raise APIPermissionError('Please signin first.')
	if not content or not content.strip():
		raise APIValueError('content')
	blog = yield from Blog.find(id)
	if blog is None:
		raise APIResourceNotFoundError('Blog')
	comment = Comment(blog_id=blog.id, user_id=user.id, user_name=user.name, user_image=user.image, content=content.strip())
	yield from comment.save()
	return comment

@post('/api/comments/{id}/delete')
def api_delete_comments(id, request):
	check_admin(request)
	c = yield from Comment.find(id)
	if c is None:
		raise APIResourceNotFoundError('Comment')
	yield from c.remove()
	return dict(id=id)
	
@get('/userinfo/{id}')
def userinfo(request, *, id):
	return {
		'__template__':'userinfo.html',
		'edit_flag':0,
		'created_date': get_datetime(request.__user__.created_at),
		'__user__': request.__user__
	}
	
@get('/userinfo/edit/{id}')
def userinfo_edit(request, *, id):
	return {
		'__template__':'userinfo.html',
		'edit_flag':1,
		'created_date': get_datetime(request.__user__.created_at),
		'__user__': request.__user__
	}
	
@get('/userinfo/save')
def userinfo_save(request, *, id):
	return {
		'__template__':'userinfo.html',
		'edit_flag':0,
		'created_date': get_datetime(request.__user__.created_at),
		'__user__': request.__user__
	}
	
@get('/mdf-passwd')
def edit_passwd(request):
	return {
		'__template__':'mdf-passwd.html',
		'__user__': request.__user__
	}
	
@get('/api/users')
def api_get_users(*, page='1'):
	page_index = get_page_index(page)
	num = yield from User.findNumber('count(id)')
	p = Page(num, page_index)
	if num == 0:
		return dict(page=p, users=())
	users = yield from User.findAll(orderBy='created_at desc', limit=(p.offset, p.limit))
	for u in users:
		u.passwd = '******'
	return dict(page=p, users=users)
	
_RE_EMAIL = re.compile(r'^[a-z0-9\.\-\_]+\@[a-z0-9\-\_]+(\.[a-z0-9\-\_]+){1,4}$')
_RE_SHA1 = re.compile(r'^[0-9a-f]{40}$')

@post('/api/users')
def api_register_user(*, email, name, passwd):
	if not name or not name.strip():
		raise APIValueError('name')
	if not email or not _RE_EMAIL.match(email):
		raise APIValueError('email')
	if not passwd or not _RE_SHA1.match(passwd):
		raise APIValueError('passwd')
	users = yield from User.findAll('email=?', [email])
	if len(users) > 0:
		raise APIError('register:failed', 'email', 'Email is already in use.')
	uid = next_id()
	sha1_passwd = '%s:%s' % (uid, passwd)
	user = User(id=uid, name=name.strip(), email = email, passwd=hashlib.sha1(sha1_passwd.encode('utf-8')).hexdigest(), image='http://www.gravatar.com/avatar/%s?d=mm&s=120' % hashlib.md5(email.encode('utf-8')).hexdigest())
	yield from user.save()
	# make session cookie:
	r = web.Response()
	r.set_cookie(COOKIE_NAME, user2cookie(user, 86400), max_age=86400, httponly=True)
	user.passwd = '******'
	r.content_type = 'application/json'
	r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
	return r

@get('/api/blogs')
def api_blogs(*, page='1'):
	page_index = get_page_index(page)
	num = yield from Blog.findNumber('count(id)')
	p = Page(num, page_index)
	if num == 0:
		return dict(page=p, blogs=())
	blogs = yield from Blog.findAll(orderBy='created_at desc', limit=(p.offset, p.limit))
	return dict(page=p, blogs = blogs)

@get('/api/blogs/{id}')
def api_get_blog(*, id):
	blog = yield from Blog.find(id)
	return blog
	
@get('/api/classes/{id}')
def api_get_classes(*, id):
	classifition = yield from Classification.find(id)
	return classifition
	
@post('/api/save/userinfo')
def api_save_userinfo(request, *, name):
	if not name or not name.strip():
		raise APIValueError('name')
	user = yield from User.find(request.__user__.id)
	user.name = name
	yield from user.update()
	
	blogs = yield from Blog.findByField('user_id',request.__user__.id)
	if blogs != None:
		for blog in blogs:
			blog.user_name = name
			yield from blog.update()
	
	comments = yield from Comment.findByField('user_id',request.__user__.id)
	if comments != None:
		for comment in comments:
			comment.user_name = name
			yield from comment.update()
	
	r = web.Response()
	r.set_cookie(COOKIE_NAME, user2cookie(user, 86400), max_age=86400, httponly=True)
	user.passwd = '******'
	r.content_type = 'application/json'
	r.body = json.dumps(user, ensure_ascii=False).encode('utf-8')
	return r
	
@post('/api/drafts')
def api_edit_draft(request, * , name, summary, content, classes_id):
	check_admin(request)
	drafts = yield from Draft.findAll(where="classes_id='%s'" % classes_id)
	if len(drafts) > 0:
		draft = yield from Draft.find(drafts[0].id)
		draft.name = name
		draft.summary = summary
		draft.content = content
		yield from draft.update()
	else:
		draft = Draft(user_id=request.__user__.id, user_name=request.__user__.name, user_image=request.__user__.image, name=name.strip(), summary=summary.strip(), content=content.strip(), classes_id=classes_id.strip())
		yield from draft.save()
	return draft
	
@get('/api/drafts/{classes_id}')
def api_get_draft(request, *, classes_id):
	check_admin(request)
	drafts = yield from Draft.findAll(where="classes_id='%s'" % classes_id)
	if len(drafts) > 0:
		draft = yield from Draft.find(drafts[0].id)
		return draft
	else:
		return {'name':''}

@post('/api/blogs')
def api_create_blog(request, * , name, summary, content, classes_id):
	check_admin(request)
	if not name or not name.strip():
		raise APIValueError('name', 'name cannot be empty.')
	if not summary or not summary.strip():
		raise APIValueError('summary', 'summary cannot be empty.')
	if not content or not content.strip():
		raise APIValueError('content', 'content cannot be empty.')
	dt = datetime.fromtimestamp(time.time())
	blog_date = '%04d%02d' % (dt.year, dt.month)
	
	classification = yield from Classification.find(classes_id)
	
	blog = Blog(user_id=request.__user__.id, user_name=request.__user__.name, user_image=request.__user__.image, name=name.strip(), summary=summary.strip(), content=content.strip(), classes_id=classes_id.strip(), classes_name = classification.name.strip(), blog_date=blog_date)
	yield from blog.save()
	
	#对应的分类的草稿要删掉
	drafts = yield from Draft.findAll(where="classes_id = '%s'" % classes_id)
	if len(drafts) == 1:
		draft = yield from Draft.find(drafts[0].id)
		yield from draft.remove()
	return blog
	
@post('/api/blogs/{id}')
def api_update_blog(id, request, *, name, summary, content, classes_id):
	check_admin(request)
	blog = yield from Blog.find(id)
	if not name or not name.strip():
		raise APIValueError('name', 'name cannot be empty.')
	if not summary or not summary.strip():
		raise APIValueError('summary', 'summary cannot be empty.')
	if not content or not content.strip():
		raise APIValueError('content', 'content cannot be empty.')
	blog.name = name.strip()
	blog.summary = summary.strip()
	blog.content = content.strip()
	blog.classes_id = classes_id.strip()
	classification = yield from Classification.find(classes_id)
	blog.classes_name = classification.name.strip()
	yield from blog.update()
	return blog
	
@post('/api/classes')
def api_create_classes(request, *, name):
	check_admin(request)
	if not name or not name.strip():
		raise APIValueError('name', 'name cannot be empty.')
	classifition = Classification(name = name.strip())
	yield from classifition.save()
	return classifition
	
@post('/api/classes/{id}')
def api_update_classes(id, request, *, name):
	check_admin(request)
	classifition = yield from Classification.find(id)
	if not name or not name.strip():
		raise APIValueError('name', 'name cannot be empty.')
	classifition.name = name.strip()
	yield from classifition.update()
	
	blogs = yield from Blog.findAll(where="classes_id = '%s'" % id)
	for eachBlog in blogs:
		blog = yield from Blog.find(eachBlog.id)
		blog.classes_name = name.strip()
		yield from blog.update()
	
	return classifition

@post('/api/blogs/{id}/delete')
def api_delete_blog(request, *, id):
	check_admin(request)
	blog = yield from Blog.find(id)
	yield from blog.remove()
	return dict(id=id)

@post('/api/classes/{id}/delete')
def api_delete_classes(request, *, id):
	check_admin(request)
	classifition = yield from Classification.find(id)
	yield from classifition.remove()
	return dict(id=id)

#用户上传头像
@post('/static/userimg')
def upload_userimg(request, *, FILES):
	filename = FILES.filename
	file = FILES.file  #文件缓存对象
	content = file.read()
	imgPath = '/static/img/' + request.__user__.id + '.' + filename
	path = '.' + imgPath
	with open(path, 'wb+') as dest:
		dest.write(content)
	#更新user表
	user = yield from User.find(request.__user__.id)
	user.image = imgPath
	yield from user.update()
	
	#更新blogs表
	blogs = yield from Blog.findAll(where="user_id = '%s'" % request.__user__.id)
	for b in blogs:
		blog = yield from Blog.find(b.id)
		blog.user_image = imgPath
		yield from blog.update()
	
	#更新comments表
	comments = yield from Comment.findAll(where="user_id = '%s'" % request.__user__.id)
	for c in comments:
		comment = yield from Comment.find(c.id)
		comment.user_image = imgPath
		yield from comment.update()
	
	return user.image

#写文章时上传图片
@post('/static/img')
def upload_img(request, *, FILES):
	filename = FILES.filename
	file = FILES.file
	content = file.read()
	imgPath = '/static/img/' + filename
	path = '.' + imgPath
	with open(path, 'wb+') as dest:
		dest.write(content)
	#如果图片名字已存在是否重新创建
	
	return imgPath
	