# -*- coding: utf-8 -*-
import BaseProcess
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