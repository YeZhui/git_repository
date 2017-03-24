#!/usr/bin/env python
# -*- coding: utf-8 -*-

#操作sqlite
#一个表：存放所有的用户 users:username
#一个表：字段：用户名、难度、所用时间  scores:username,difficulty,time

import sqlite3
import os

class db_sqlite:
	def __init__(self,  dbname):
		#如果文件不存在则建立数据库和表否则打开就好
		self.m_Con = sqlite3.connect(dbname)
		self.m_Cu = self.m_Con.cursor()
		self.m_Diffi = 0  #默认难度
		if (os.path.exists(dbname) == False):
			self.createTable()
		
	def __del__(self):
		self.close()
		
	def createTable(self):
		self.m_Cu.execute("create table users(username varchar(32) primary key)")
		self.m_Cu.execute("create table scores(username varchar(32), difficulty integer,  time integer)")
		self.m_Con.commit()
		
	def insertUsers(self,  username):
		listSql = ['select * from users where username=', "'",  str(username),  "'"]
		strSql = ''.join(listSql)
		self.m_Cu.execute(strSql)
		result = self.m_Cu.fetchall()
		if (len(result) == 0):
			listSql = ['insert into users values(', "'", username, "'", ')']
			strSql = ''.join(listSql)
			self.m_Cu.execute(strSql)
			self.m_Con.commit()
			
	def insertScores(self,  username,  difficulty,  time):
		t = (username, difficulty,  time)
		self.m_Cu.execute("insert into scores values(?, ?, ?)", t)
		self.m_Con.commit()
		
	def listUsers(self):
		self.m_Cu.execute("select * from users")
		result = self.m_Cu.fetchall()
		return result
		
	def calRanking(self,  difficulty):
		listSql = ['select username, min(time) from scores where difficulty=',  str(difficulty), ' group by username order by min(time) asc']
		strSql = ''.join(listSql)
		self.m_Cu.execute(strSql)
		result = self.m_Cu.fetchall()
		return result
		
	def close(self):
		self.m_Cu.close()
		self.m_Con.close()

if __name__ == '__main__':
	print('***********')
	db = db_sqlite('./user.db')
	print('============')
	#db.insertScores('aa', 1, 9)
	#db.insertScores('bb', 1, 9)
	#result = db.listUsers()
	print('-----')
	#print(result)
	result = db.calRanking(1)
	print(result)
