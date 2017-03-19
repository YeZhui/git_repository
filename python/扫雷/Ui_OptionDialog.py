# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'D:\python源代码\扫雷\OptionDialog.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtWidgets
from db_sqlite3 import db_sqlite
from PyQt5.QtWidgets import QTableWidget, QTableWidgetItem

class Communicate(QtCore.QObject):
	clickApp = QtCore.pyqtSignal()

class OptionDialog(QtWidgets.QDialog):
	def __init__(self, com):
		super().__init__()
		self.m_Com = com    #自定义信号
		self.setupUi()
	
	def setupUi(self):
		self.setFixedSize(410, 400)
		self.setSizeGripEnabled(True)
		
		groupBox = QtWidgets.QGroupBox(self)
		groupBox.setGeometry(QtCore.QRect(10, 10, 380, 211))
		groupBox.setTitle(QtCore.QCoreApplication.translate("OptionDialog", "难度"))
		self.m_PriRadioButton = QtWidgets.QRadioButton(groupBox)
		self.m_PriRadioButton.setGeometry(QtCore.QRect(20, 20, 161, 51))
		self.m_MidRadioButton = QtWidgets.QRadioButton(groupBox)
		self.m_MidRadioButton.setGeometry(QtCore.QRect(20, 75, 161, 51))
		self.m_AdvRadioButton = QtWidgets.QRadioButton(groupBox)
		self.m_AdvRadioButton.setGeometry(QtCore.QRect(20, 135, 141, 61))
		self.m_SelfRadioButton = QtWidgets.QRadioButton(groupBox)
		self.m_SelfRadioButton.setGeometry(QtCore.QRect(170, 30, 141, 21))
		
		self.label = QtWidgets.QLabel(groupBox)
		self.label.setGeometry(QtCore.QRect(190, 60, 61, 20))
		self.label_2 = QtWidgets.QLabel(groupBox)
		self.label_2.setGeometry(QtCore.QRect(190, 90, 61, 20))
		self.label_3 = QtWidgets.QLabel(groupBox)
		self.label_3.setGeometry(QtCore.QRect(190, 120, 61, 20))
		
		self.m_HeiSize = QtWidgets.QLineEdit(groupBox)
		self.m_HeiSize.setGeometry(QtCore.QRect(260, 60, 113, 20))
		self.m_WeiSize = QtWidgets.QLineEdit(groupBox)
		self.m_WeiSize.setGeometry(QtCore.QRect(260, 90, 113, 20))
		self.m_MineSize = QtWidgets.QLineEdit(groupBox)
		self.m_MineSize.setGeometry(QtCore.QRect(260, 120, 113, 20))
		
		self.m_ShowBox = QtWidgets.QCheckBox(self)
		self.m_ShowBox.setGeometry(QtCore.QRect(40, 244, 71, 16))
		self.m_SoundBox = QtWidgets.QCheckBox(self)
		self.m_SoundBox.setGeometry(QtCore.QRect(40, 266, 71, 16))
		self.m_SaveBox = QtWidgets.QCheckBox(self)
		self.m_SaveBox.setGeometry(QtCore.QRect(40, 288, 107, 16))
		self.m_PromitBox = QtWidgets.QCheckBox(self)
		self.m_PromitBox.setGeometry(QtCore.QRect(40, 310, 71, 16))
		
		cancelButton = QtWidgets.QPushButton(self)
		cancelButton.setGeometry(QtCore.QRect(280, 350, 101, 23))
		pushButton = QtWidgets.QPushButton(self)
		pushButton.setGeometry(QtCore.QRect(160, 350, 101, 23))
		cancelButton.setText("取消")
		pushButton.setText("确定")

		self.retranslateUi()
		pushButton.clicked.connect(self.clickOk)
		cancelButton.clicked.connect(self.clickCancel)
		
	def clickCancel(self):
		r = 0
		self.done(r)
		
	def clickOk(self):
		#根据选择的不同成员变量赋不同的值
		if (self.m_PriRadioButton.isChecked()):
			self.m_Height = 9
			self.m_Width = 9
			self.m_MineNum = 16
			self.m_CurDiffi = 0
		elif(self.m_MidRadioButton.isChecked()):
			self.m_Height = 16
			self.m_Width = 16
			self.m_MineNum = 36
			self.m_CurDiffi = 1
		elif(self.m_AdvRadioButton.isChecked()):
			self.m_Height = 16
			self.m_Width = 32
			self.m_MineNum = 99
			self.m_CurDiffi = 2
		else:
			#从lineEdit里面取值
			self.m_Height = int(self.m_HeiSize.text())
			self.m_Width = int(self.m_WeiSize.text())
			self.m_MineNum = int(self.m_MineSize.text())
			self.m_CurDiffi = 3
		self.m_Com.clickApp.emit()

	def retranslateUi(self):
		_translate = QtCore.QCoreApplication.translate   #用于中文翻译
		self.setWindowTitle(_translate("OptionDialog", "选项"))
		self.m_PriRadioButton.setText(_translate("OptionDialog", "初级(B)\n"
"10个雷\n"
"9 X 9平铺网络"))
		self.m_MidRadioButton.setText(_translate("OptionDialog", "中级(I)\n"
"40个雷\n"
"16 X 16平铺网络"))
		self.m_AdvRadioButton.setText(_translate("OptionDialog", "高级(V)\n"
"99个雷\n"
"16 X 30平铺网络"))
		self.m_SelfRadioButton.setText(_translate("OptionDialog", "自定义(U)"))
		self.label.setText(_translate("OptionDialog", "<html><head/><body><p>高度(9-24)(H):</p></body></html>"))
		self.label_2.setText(_translate("OptionDialog", "<html><head/><body><p>宽度(9-30)(W):</p></body></html>"))
		self.label_3.setText(_translate("OptionDialog", "<html><head/><body><p>雷数(10-99)(M):</p></body></html>"))
		self.m_HeiSize.setText(_translate("OptionDialog", "9"))
		self.m_WeiSize.setText(_translate("OptionDialog", "9"))
		self.m_MineSize.setText(_translate("OptionDialog", "16"))
		self.m_ShowBox.setText(_translate("OptionDialog", "显示动画"))
		self.m_SoundBox.setText(_translate("OptionDialog", "播放声音"))
		self.m_SaveBox.setText(_translate("OptionDialog", "退出时保存游戏"))
		self.m_PromitBox.setText(_translate("OptionDialog", "显示提示"))
		
class scoreDialog(QtWidgets.QDialog):
	def __init__(self,  db):
		super().__init__()
		self.m_Db = db
		self.initUI()
		
	def initUI(self):
		self.setFixedSize(450, 450)
		_translate = QtCore.QCoreApplication.translate   #用于中文翻译
		self.setWindowTitle(_translate("OptionDialog", "用户排名"))
		juniorBtn = QtWidgets.QPushButton(self)
		juniorBtn.setGeometry(QtCore.QRect(30, 30, 80, 23))
		middleBtn = QtWidgets.QPushButton(self)
		middleBtn.setGeometry(QtCore.QRect(120, 30, 80, 23))
		seniorBtn = QtWidgets.QPushButton(self)
		seniorBtn.setGeometry(QtCore.QRect(210, 30,  80,  23))
		userBtn = QtWidgets.QPushButton(self)
		userBtn.setGeometry(QtCore.QRect(300, 30,  80,  23))
		juniorBtn.setText("初级难度排名")
		middleBtn.setText("中级难度排名")
		seniorBtn.setText("高级难度排名")
		userBtn.setText("显示所有用户")
		
		juniorBtn.clicked.connect(self.juniorSlot)
		middleBtn.clicked.connect(self.middleSlot)
		seniorBtn.clicked.connect(self.seniorSlot)
		userBtn.clicked.connect(self.userSlot)
		
		self.m_Table = QTableWidget(self)
		self.m_Table.setRowCount(4)
		self.m_Table.setColumnCount(4)
		#self.m_Table.verticalHeader().setVisible(False)
		#self.m_Table.horizontalHeader().setVisible(False)
		self.m_Table.setGeometry(QtCore.QRect(30,  60,  390,  350))
		
	def getScores(self,  diffi):
		result = self.m_Db.calRanking(diffi)
		self.m_Table.clear()
		self.m_Table.setRowCount(0)
		self.m_Table.setColumnCount(2)
		self.m_Table.setHorizontalHeaderLabels(['用户', '用时'])
		for lst in result:
			curRow = self.m_Table.rowCount()
			self.m_Table.insertRow(curRow)
			column=0
			for item in lst:
				newItem = QTableWidgetItem(str(item))
				self.m_Table.setItem(curRow,  column,  newItem)
				column += 1
				
	def getUsers(self):
		result = self.m_Db.listUsers()
		self.m_Table.clear()
		self.m_Table.setRowCount(0)
		self.m_Table.setColumnCount(1)
		self.m_Table.setHorizontalHeaderLabels(['用户'])
		for lst in result:
			curRow = self.m_Table.rowCount()
			self.m_Table.insertRow(curRow)
			column=0
			for item in lst:
				newItem = QTableWidgetItem(str(item))
				self.m_Table.setItem(curRow,  column,  newItem)
				column += 1
				
	def juniorSlot(self):
		self.getScores(0)
		
	def middleSlot(self):
		self.getScores(1)
		
	def seniorSlot(self):
		self.getScores(2)
		
	def userSlot(self):
		self.getUsers()

if __name__ == "__main__":
	import sys
	app = QtWidgets.QApplication(sys.argv)
	db = db_sqlite('./user.db')
	
	option = scoreDialog(db)
	option.show()
	sys.exit(app.exec_())

