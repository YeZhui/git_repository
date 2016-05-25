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
	//配置文件读取日志文件存放路径
	//打开文件
	char szFileName[128] = {0};
	strncpy(m_szFilePath, ".", sizeof(m_szFilePath)-1);
	snprintf(szFileName, sizeof(szFileName)-1, "%s/runlog.log", m_szFilePath);
	m_file.open(m_szFileName, ios::out|ios::app);
}

int CFileLog::ReOpen()
{
	//先检查文件大小，超过50M就写下一个文件
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
			//每写100条检查文件大小，文件刚打开时检查文件大小
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
	//关闭文件
	m_file.close();
}

