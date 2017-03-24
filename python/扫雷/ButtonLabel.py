#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import QLabel,  QFrame, QTimeEdit
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtCore import Qt, QTime, QTimer

class TimeEdit(QTimeEdit):
	def __init__(self):
		super().__init__(QTime.currentTime())
		#self.setMinimumTime(QTime.currentTime())   #定义了用户可以设置的最小时间
		self.setDisplayFormat("HH:mm:ss")
		self.setFixedSize(100, 30)
		self.setStyleSheet("QTimeEdit{background-color:rgb(255, 255, 255, 120)}")
		#设置定时器，这里的timer一定要是成员变量或是全局变量
		self.setReadOnly(True)  #编辑框设为只读
		self.timer = QTimer()
		self.timer.timeout.connect(self.UpdateTime)
		self.timer.start(1000)
	
	def UpdateTime(self):
		self.setTime(QTime.currentTime())

class NumLabel(QLabel):
	def __init__(self, title):
		super().__init__(title)
		self.adjustSize()  #自适应文本的大小
		self.setAutoFillBackground(True)
		p = self.palette()
		p.setColor(QPalette.Window, QColor(255, 255, 255, 120))
		p.setColor(QPalette.WindowText, QColor(0, 0, 0, 255))
		self.setPalette(p)   #设置控件的颜色
		self.setFrameStyle(QFrame.Panel | QFrame.Sunken)   #设置控件的样式
		self.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)    #设置控件中文本的位置
		self.setFixedSize(40, 35)

class ButtonLabel(QLabel):
	m_Score = 0
	m_Pos = [-1, -1]  #所有实例共有的变量，标识当前点击的是哪个label
	def __init__(self, title, singleSize):
		super().__init__(title)
		self.m_Value = 0  #0为初始值，计算周围雷的数量后改变，是雷则改为-1
		self.m_Flag = 0   #表示该按钮是否被点击过了
		self.initUI()
		self.setFixedSize(singleSize, singleSize)
		
	def initUI(self):
		self.setAutoFillBackground(True)
		self.m_Palette = self.palette()
		self.m_Palette.setColor(QPalette.Window, QColor(141, 182, 205, 250))
		self.m_Palette.setColor(QPalette.WindowText, QColor(255, 255, 255, 255))
		self.setPalette(self.m_Palette) 
		self.setFrameStyle(QFrame.StyledPanel | QFrame.Sunken)
		self.setLineWidth(0)
		self.setMidLineWidth(0)
		self.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)   #设置控件中文本居中
		
	def reset(self):
		self.m_Value = 0
		self.m_Flag = 0
		self.initUI()
		ButtonLabel.m_Score = 0
		ButtonLabel.m_Pos = [-1, -1]
		self.setText('')
		
	def setPos(self,  x,  y):
		self.m_x = x
		self.m_y = y
		
	def display(self):
			self.m_Flag = 1
			if self.m_Value == 0:
				self.setText('')
			else:
				self.setText(str(self.m_Value))
			self.setFrameStyle(QFrame.WinPanel | QFrame.Sunken)
			#暂时先统一颜色，之后可以改为根据数字的不同设定不同的颜色
			self.m_Palette.setColor(QPalette.Window, QColor(255, 255, 255, 120))
			self.m_Palette.setColor(QPalette.WindowText, QColor(0, 0, 0, 255))
			self.setPalette(self.m_Palette)
			ButtonLabel.m_Score += 1
		
	def mousePressEvent(self, e):
		if self.m_Flag == 1:
			return
		QLabel.mousePressEvent(self, e)
		if e.button() == Qt.LeftButton:
			self.display()
			if self.m_Value == 0:
				ButtonLabel.m_Pos = [self.m_x, self.m_y]
			elif self.m_Value == 'X':
				ButtonLabel.m_Pos = ['X', 'Y']
				ButtonLabel.m_Score = 0
		else:
			text = self.text()
			#之后要改为点击右键设置图标
			if text == 'X':
				self.m_Palette.setColor(QPalette.Window, QColor(141, 182, 205, 250))
				self.setText('')
			else:
				self.m_Palette.setColor(QPalette.Window, QColor(0, 0, 255, 120))
				self.setText('X')
			self.setPalette(self.m_Palette)
