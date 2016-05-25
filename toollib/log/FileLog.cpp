#include "BaseLog.h"
#include "FileLog.h"
#include <stdio.h>

CFileLog::CFileLog()
{
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
	Init();
}

CFileLog::~CFileLog()
{
	Close();
}

void CFileLog::Init()
{
	//�����ļ���ȡ��־�ļ����·��
	//���ļ�
	char szFileName[128] = {0};
	strncpy(m_szFilePath, ".", sizeof(m_szFilePath)-1);
	snprintf(szFileName, sizeof(szFileName)-1, "%s/runlog.log", m_szFilePath);
	m_file.open(m_szFileName, ios::out|ios::app);
}

int CFileLog::ReOpen()
{
	//�ȼ���ļ���С������50M��д��һ���ļ�
	//Close();
	return 0;
}


int CFileLog::ReadAndPrint()
{
	string strLog;
	int count = 0;
	while(1)
	{
		pthread_mutex_lock(&m_mutex);
		if (m_queLog.empty())
		{
			pthread_cond_wait(&m_cond, &m_mutex);
		}
		while(!m_queLog.empty())
		{
			strLog.clear();
			strLog = m_queLog.front();
			m_queLog.pop();
			//ÿд100������ļ���С���ļ��մ�ʱ����ļ���С
			m_file << strLog.c_str() << endl;
			count++;
			if (count == 100)
			{
				ReOpen();
				count = 0;
			}
		}
		pthread_mutex_unlock(&m_mutex);
	}
	return 0;
}

void CFileLog::Close()
{
	//�ر��ļ�
	m_file.close();
}

