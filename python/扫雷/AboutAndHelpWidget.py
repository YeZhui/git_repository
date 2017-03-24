#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLabel
		
class AboutWidget(QWidget):
	def __init__(self):
		super().__init__()
		vLayout = QVBoxLayout()
		label = QLabel("这是一个学习pyqt时写的第一个程序\n作者是霞霞的老公\nV1.0.0.0")
		vLayout.addWidget(label)
		self.setLayout(vLayout)
		self.resize(300, 80)
		self.move(350, 350)
		self.setWindowTitle('关于扫雷')
		self.setStyleSheet("QWidget{color:black;background-color:rgb(162, 181, 205, 255)}")
		self.show()
		
#帮助文档可以弄成一个文档或是打开类似于浏览器的东西
class HelpWidget(QWidget):
	def __init__(self):
		super().__init__()
		self.resize(300, 80)
		self.move(350, 350)
		self.setWindowTitle("帮助文档")
		vLayout = QVBoxLayout()
		label = QLabel("具体操作情况请询问牛的主人!")
		vLayout.addWidget(label)
		self.setLayout(vLayout)
		self.setStyleSheet("QWidget{color:black;background-color:rgb(162, 181, 205, 255)}")
		self.show()
