# -*- coding: utf-8 -*-
import BaseProcess
"""
һ�ײ���sqlite3�ķ���
��Ҫ����sql����ķ���
"""

class Sqlite3Proc():
	__conn__ = sqlite3.connect('./win7sh.db')
	def __init__(self):
		self.m_Cursor = Sqlite3Proc.__conn__.cursor()

	def exeSqlCmd(self,strSql):
		self.m_Cursor.execute(strSql)
		
	def getRowCount(self):
		return self.m_Cursor.rowcount
		
	def closeCursor(self):
		self.m_Cursor.close()
		
	def commit(self):
		Sqlite3Proc.__conn__.commit()
		
	def closeConn(self):
		Sqlite3Proc.__conn__.close()
		
	def getQeuryRet(self):
		return m_Cursor.fetchall()