# -*- coding: utf-8 -*-
import BaseProcess
import os
"""
解析各种操作文件的命令
来一个入参，然后解析后调用对应的函数处理
返回结果写入全局变量
命令组：
cd    切换目录
mkdir 建立目录
touch 新建文件（如果文件已存在，报错）
rm    删除目录或者文件
open  打开文件
cp    复制目录或者文件
mv    移动目录或者文件
"""

def processFileCmd(strFileCmd):
	if strFileCmd[0:3] == 'cd ':
		processCdCmd(strFileCmd)
	elif strFileCmd[0:6] == 'mkdir ':
		processMkdirCmd(strFileCmd)
	elif strFileCmd[0:6] == 'touch ':
		processTouchCmd(strFileCmd)
	elif strFileCmd[0:3] == 'rm ':
		processRmCmd(strFileCmd)
	elif strFileCmd[0:5] == 'open ':
		processOpenCmd(strFileCmd)
	elif strFileCmd[0:3] == 'cp ':
		processCpCmd(strFileCmd)
	elif strFileCmd[0:3] == 'mv ':
		processMvCmd(strFileCmd)
	else:
		BaseProcess.g_ErrorCode = -1
		BaseProcess.g_ErrorMsg = "The cmmond is not found!"
		
def getAbsPath(strCmd):
	#要考虑字符串长度
	curPath = os.path.abspath('.')
	if len(strCmd) == 0:
		absPath = curPath
	elif len(strCmd) == 1:
		if strCmd == '.':
			absPath = curPath
		else:
			BaseProcess.g_ErrorCode = -1
			BaseProcess.g_ErrorMsg = "This Dir is not exists!"
			return None
	elif len(strCmd) >=2 and strCmd[1] != ':':
		absPath = os.path.join(curPath,strCmd)
	else:
		absPath = strCmd
	return absPath
		
def processCdCmd(strFileCmd):
	"""
	分为当前路径和绝对路径r
	以字母+冒号开头的都是绝对路径，否则就是相对路径
	首先识别相对路径，然后转换为绝对路径操作
	"""
	strCdCmd = strFileCmd[3:]
	strCdCmd.lstrip()     #截掉string左边的空格
	try:
		absPath = getAbsPath(strCdCmd)
		if absPath:
			if os.path.isdir(absPath):
				os.chdir(absPath)  #切换目录
				BaseProcess.g_ErrorCode = 0
			else:
				BaseProcess.g_ErrorCode = -1
				BaseProcess.g_ErrorMsg = absPath + ' not a dir'
	except OSError:
		BaseProcess.g_ErrorCode = -1
		BaseProcess.g_ErrorMsg = "OS Error"
	
def processMkdirCmd(strFileCmd):
	strMkdirCmd = strFileCmd[6:]
	strMkdirCmd.lstrip()
	try:
		absPath = getAbsPath(strCdCmd)
		if absPath:
			if os.path.exists(absPath):
				BaseProcess.g_ErrorCode = -1
				BaseProcess.g_ErrorMsg = absPath + 'is already exists!'
			else:
				os.path.mkdir(absPath)
				BaseProcess.g_ErrorCode = 0
	except OSError:
		BaseProcess.g_ErrorCode = -1
		BaseProcess.g_ErrorMsg = "OS Error"
		
def processRmCmd(strFileCmd):
	strRmCmd = strFileCmd[3:]
	strRmCmd.lstrip()
	try:
		absPath = getAbsPath(strRmCmd)
		if absPath:
			if os.path.isdir(absPath):
				os.rmdir(absPath)
			elif os.path.exists(absPath):
				os.remove(absPath)
			else:
				BaseProcess.g_ErrorCode = -1
				BaseProcess.g_ErrorMsg = absPath + 'is not exists!'
	except OSError:
		BaseProcess.g_ErrorCode = -1
		BaseProcess.g_ErrorMsg = "OS Error"
	BaseProcess.g_ErrorCode = 0
		
	
def processTouchCmd(strFileCmd):
	strTouchCmd = strFileCmd[6:]
	strTouchCmd.lstrip()
	try:
		absPath = getAbsPath

def processOpenCmd(strFileCmd):
	pass
def processCpCmd(strFileCmd):
	pass
def processMvCmd(strFileCmd):
	pass