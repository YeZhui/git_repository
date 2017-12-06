# -*- coding:utf-8 -*-
import logging
from logging.handlers import RotatingFileHandler

runLog = '../log/runlog.log'

def logConfig(filename):
	#调试时debug以上信息都输出
	logging.basicConfig(level=logging.DEBUG,
		format = '%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
	#定义最多备份5个日志文件，每个日志文件最大10M
	rtHandler = RotatingFileHandler(filename, mode='a', maxBytes=10*1024*1024, backupCount=100, encoding='utf-8')
	#只有警告级别以上日志才写入文件
	rtHandler.setLevel(logging.WARNING)
	formatter = logging.Formatter('%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s')
	rtHandler.setFormatter(formatter)
	log = logging.getLogger('')
	log.addHandler(rtHandler)

if __name__ == '__main__':
	logConfig('my.log')
	logging.debug('This is debug message')
	logging.error('This is error message')
