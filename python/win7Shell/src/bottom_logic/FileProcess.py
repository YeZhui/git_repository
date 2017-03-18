# -*- coding: utf-8 -*-
import BaseProcess
import os
"""
�������ֲ����ļ�������
��һ����Σ�Ȼ���������ö�Ӧ�ĺ�������
���ؽ��д��ȫ�ֱ���
�����飺
cd    �л�Ŀ¼
mkdir ����Ŀ¼
touch �½��ļ�������ļ��Ѵ��ڣ�����
rm    ɾ��Ŀ¼�����ļ�
open  ���ļ�
cp    ����Ŀ¼�����ļ�
mv    �ƶ�Ŀ¼�����ļ�
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
	#Ҫ�����ַ�������
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
	��Ϊ��ǰ·���;���·��r
	����ĸ+ð�ſ�ͷ�Ķ��Ǿ���·��������������·��
	����ʶ�����·����Ȼ��ת��Ϊ����·������
	"""
	strCdCmd = strFileCmd[3:]
	strCdCmd.lstrip()     #�ص�string��ߵĿո�
	try:
		absPath = getAbsPath(strCdCmd)
		if absPath:
			if os.path.isdir(absPath):
				os.chdir(absPath)  #�л�Ŀ¼
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