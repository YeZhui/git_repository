# -*- coding: utf-8 -*-
import BaseProcess
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
	else
		BaseProcess.g_ErrorMsg = "The cmmond is not found!"
		
def processCdCmd(strFileCmd):
	pass
	
def processMkdirCmd(strFileCmd):
	pass
def processTouchCmd(strFileCmd):
	pass
def processRmCmd(strFileCmd):
	pass
def processOpenCmd(strFileCmd):
	pass
def processCpCmd(strFileCmd):
	pass
def processMvCmd(strFileCmd):
	pass