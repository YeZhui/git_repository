#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
import sys
from PyQt5.QtWidgets import QInputDialog
from PyQt5.QtWidgets import QMainWindow, QApplication, QAction, QPushButton, QWidget
from PyQt5.QtWidgets import QGridLayout, QHBoxLayout, QVBoxLayout,  QMessageBox, qApp
from ButtonLabel import ButtonLabel, NumLabel, TimeEdit
from AboutAndHelpWidget import AboutWidget, HelpWidget
from Ui_OptionDialog import OptionDialog, Communicate
from db_sqlite3 import db_sqlite
from PyQt5.QtGui import QIcon, QPalette, QColor

class MineSweeping(QMainWindow):
	m_SingleSize = 300.0/9.0     #单个雷区的大小
	m_Space = 3.5
	m_User = 'admin'
	m_Db = db_sqlite('./user.db')
	m_Diffi = 0
	def __init__(self):
		super().__init__()
		self.addMenuBar()             #添加菜单
		self.initSize(9, 9, 16)         #初始化窗体大小
		
		#初始化一些始终存在的控件
		#self.m_TimeQle = NumLabel('0')	
		self.m_TimeQle = TimeEdit()
		self.m_ScoreQle = NumLabel('0')
		self.m_Widget = QWidget(self)
		space = (MineSweeping.m_SingleSize*MineSweeping.m_Space)/2
		self.m_Widget.setGeometry(space, space, MineSweeping.m_SingleSize*self.m_RowNum, self.m_WinHeight-space)
		
		self.initUI()                      #初始化窗体控件(所有扫雷区和分数、时间控件)
		self.initSweepingData()		 #初始化产生所有的雷区
		self.m_Widget.show()
		
		#self.userSlot()
		
	def resetUI(self, lineNum, rowNum, mineNum): 
		self.initSize(lineNum, rowNum, mineNum)
		
		self.m_Widget.close()
		self.m_Widget = QWidget(self)
		space = (MineSweeping.m_SingleSize*MineSweeping.m_Space)/2
		self.m_Widget.move(space, space)
		
		self.initUI()
		self.initSweepingData()
		self.m_Widget.show()
		
	def initSize(self, lineNum, rowNum, mineNum):
		self.m_LineNum = lineNum       #扫雷区行数
		self.m_RowNum = rowNum      #扫雷区列数
		self.m_MineNum = mineNum    #雷的数量
		self.m_WinLength = MineSweeping.m_SingleSize*(self.m_RowNum+MineSweeping.m_Space)     #窗体长度
		self.m_WinHeight = MineSweeping.m_SingleSize*(self.m_LineNum+MineSweeping.m_Space)      #窗体高度
		self.m_ButtonDict = {}

	def mousePressEvent(self, e):
		if (ButtonLabel.m_Pos[0] == 'X') and (ButtonLabel.m_Pos[1] == 'Y'):
			rb = QMessageBox.question(self, 'Message', "失败，是否再来一局?", QMessageBox.Yes|QMessageBox.No, QMessageBox.No)
			#当点击是时，调用reset
			if (rb == QMessageBox.Yes):
				self.newGameSlot()
			else:
				qApp.quit()   #退出程序
			ButtonLabel.m_Pos = [-1, -1]
			return
		if (ButtonLabel.m_Pos[0] != -1) and (ButtonLabel.m_Pos[1] != -1):
			self.openAround(ButtonLabel.m_Pos)
			ButtonLabel.m_Pos = [-1, -1]
			if (ButtonLabel.m_Score == self.m_LineNum*self.m_RowNum-self.m_MineNum):
				QMessageBox.question(self, 'Message', "恭喜你，你赢了!")
				#赢了，胜利记录写入数据库，难度为3即自定义时，不记录
				#MineSweeping.m_Db.insertScores()  MineSweeping.m_Diffi
				#弹出下一个对话框，询问是否进行下一盘，是，调用reset，否则调用quit函数退出
				rb = QMessageBox.question(self, 'Message', "是否继续?", QMessageBox.Yes|QMessageBox.No, QMessageBox.No)
				if (rb == QMessageBox.Yes):
					self.newGameSlot()
				else:
					qApp.quit()
		self.m_ScoreQle.setText(str(ButtonLabel.m_Score))

	def initUI(self):
		grid = QGridLayout()
		grid.setSpacing(0)
		for i in range(self.m_LineNum):
			buttonLst = []
			for j in range(self.m_RowNum):
				button = ButtonLabel('', MineSweeping.m_SingleSize)
				button.setPos(i, j)
				buttonLst.append(button)
				grid.addWidget(button, i, j)
				self.m_ButtonDict[(i, j)] = button
			
		hbox = QHBoxLayout()
		timeButton = QPushButton("时间:")
		timeButton.setFixedSize(50, 35)
		scoreButton = QPushButton("得分:")
		scoreButton.setFixedSize(50, 35)
		hbox.addWidget(timeButton, 0)
		hbox.addWidget(self.m_TimeQle,  1)
		hbox.addWidget(scoreButton,  2)
		hbox.addWidget(self.m_ScoreQle,  3)
		vbox = QVBoxLayout()
		vbox.addLayout(grid)
		vbox.addLayout(hbox)
		self.m_Widget.setLayout(vbox)
		self.m_Widget.resize(MineSweeping.m_SingleSize*self.m_RowNum, MineSweeping.m_SingleSize*(self.m_LineNum+MineSweeping.m_Space/2))
		#设置主界面的样式
		pe1 = QPalette()
		self.setAutoFillBackground(True)
		pe1.setColor(QPalette.Window, QColor(162, 181, 205, 255))
		self.setPalette(pe1)
		#设置widget的样式
		pe2 = QPalette()
		self.m_Widget.setAutoFillBackground(True)
		pe2.setColor(QPalette.Window, QColor(162, 181, 205,  255))
		self.m_Widget.setPalette(pe2)
		
		self.setFixedSize(self.m_WinLength, self.m_WinHeight)
		self.move((QApplication.desktop().width()-self.width())/2, (QApplication.desktop().height()-self.height())/2)
		self.setWindowTitle('扫雷')
		self.setWindowIcon(QIcon(r'D:\python源代码\扫雷\icon\mine.ico'))
		self.show()
		
	def initSweepingData(self):
		sweepingLst = random.sample(self.m_ButtonDict.keys(),self.m_MineNum)
		for pos in sweepingLst:
			self.m_ButtonDict[(pos[0], pos[1])].m_Value = 'X'
			for i in range( max(0,pos[0]-1),min(pos[0]+1,self.m_LineNum-1 )+1):
				for j in range( max(0,pos[1]-1),min(pos[1]+1,self.m_RowNum-1)+1 ):
					if self.m_ButtonDict[(i, j)].m_Value != 'X':
						self.m_ButtonDict[(i, j)].m_Value += 1
						
	def openAround(self, curPos):
		newZeroLst = [curPos]
		while newZeroLst:
			pos = newZeroLst.pop(0)
			for i in range( max(0,pos[0]-1),min(pos[0]+1,self.m_LineNum-1)+1):
				for j in range(max(0,pos[1]-1),min(pos[1]+1,self.m_RowNum-1)+1):
					if (self.m_ButtonDict[(i, j)].m_Flag == 1):
						continue
					else:
						self.m_ButtonDict[(i, j)].display()
						if self.m_ButtonDict[(i, j)].m_Value == 0:
							newZeroLst.append([i,j])
		
	def addMenuBar(self):
		menu = self.menuBar()
		gameMenu = menu.addMenu('&游戏(G)')
		newGameAction = QAction('&新游戏', self)
		newGameAction.setShortcut('F2')
		newGameAction.setStatusTip('重新开始新的一局游戏')
		newGameAction.triggered.connect(self.newGameSlot)
		gameMenu.addAction(newGameAction)
		
		optionAction = QAction('&选项', self)
		optionAction.setShortcut('F3')
		optionAction.setStatusTip('设置扫雷游戏的难度')
		optionAction.triggered.connect(self.optionSlot)
		gameMenu.addAction(optionAction)
		
		userAction = QAction('&用户配置',  self)
		userAction.setShortcut('ctrl+w')
		userAction.setStatusTip('设置当前用户名')
		userAction.triggered.connect(self.userSlot)
		gameMenu.addAction(userAction)
		
		scoreAction = QAction('&排名与用户查询', self)
		scoreAction.setShortcut('ctrl+v')
		scoreAction.setStatusTip('显示不同难度用户排名')
		scoreAction.triggered.connect(self.scoreSlot)
		gameMenu.addAction(scoreAction)
		
		helpMenu = menu.addMenu('&帮助(H)')
		helpAction = QAction('&查看帮助', self)
		helpAction.setShortcut('F4')
		helpAction.setStatusTip('点击显示帮助文档')
		helpAction.triggered.connect(self.helpActionSlot)
		helpMenu.addAction(helpAction)
		aboutAction = QAction('&关于扫雷', self)
		aboutAction.setShortcut('F5')
		aboutAction.triggered.connect(self.aboutActionSlot)
		helpMenu.addAction(aboutAction)
	
	def newGameSlot(self):
		#所有的雷区回复初始位
		for i in range(self.m_LineNum):
			for j in range(self.m_RowNum):
				self.m_ButtonDict[(i, j)].reset()
		#时钟和得分回到初始位
		self.m_ScoreQle.setText(str(ButtonLabel.m_Score))
		#所有的雷要重新计算
		self.initSweepingData()
		
	def optionSlot(self):
		#设计一个dialog类，设置坐标和雷的数量
		optionSignal = Communicate()
		optionSignal.clickApp.connect(self.optionChange)
		self.m_OptionDialog = OptionDialog(optionSignal)
		self.m_OptionDialog.show()
		#然后根据输入的值，修改成员变量和实例变量，保证所有的情况都是初始化的
		return
		
	def userSlot(self):
		text,  ok = QInputDialog.getText(self,  '用户名配置', '如点击取消将使用默认的用户(admin)\n'
		'请输入您的用户名:')
		if ok:
			if len(str(text)) == 0:
				MineSweeping.m_User = 'admin'
			else:
				MineSweeping.m_User = str(text)
		else:
			MineSweeping.m_User = 'admin'
		MineSweeping.m_Db.insertUsers(MineSweeping.m_User)
		
	def scoreSlot(self):
		return
	
	def helpActionSlot(self):
		self.m_HelpWidget = HelpWidget()

	def aboutActionSlot(self):
		self.m_AboutWidget = AboutWidget()
		
	def optionChange(self):
		lineNum= self.m_OptionDialog.m_Height
		rowNum = self.m_OptionDialog.m_Width
		mineNum = self.m_OptionDialog.m_MineNum
		MineSweeping.m_Diffi = self.m_OptionDialog.m_CurDiffi
		self.m_OptionDialog.done(0)
		self.resetUI(lineNum, rowNum, mineNum)
		
if __name__ == '__main__':
	app = QApplication(sys.argv)
	mineSweeping = MineSweeping()
	sys.exit(app.exec_())
