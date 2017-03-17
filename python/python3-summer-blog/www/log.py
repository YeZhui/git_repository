# -*- coding:utf-8 -*-
import logging
from logging.handlers import RotatingFileHandler

def logConfig(filename):
	#����ʱdebug������Ϣ�����
	logging.basicConfig(level=logging.DEBUG,
		format = '%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
	#������౸��5����־�ļ���ÿ����־�ļ����10M
	rtHandler = RotatingFileHandler(filename, mode='a', maxBytes=10*1024*1024, backupCount=100, encoding='utf-8')
	#ֻ�д�����־��д���ļ�
	rtHandler.setLevel(logging.WARNING)
	formatter = logging.Formatter('%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
	rtHandler.setFormatter(formatter)
	log = logging.getLogger('')
	log.addHandler(rtHandler)

def logOutput(sql,args=(),level='debug'):
	if level == 'debug':
		logging.debug('SQL: %s' %(sql))
	elif level == 'info':
		logging.info('SQL: %s' %(sql))
	elif level == 'warning':
		logging.warning('SQL: %s' %(sql))
	else:
		logging.error('SQL: %s' %(sql))

if __name__ == '__main__':
	logConfig('my.log')
	logging.debug('This is debug message')
	logging.error('This is error message')
