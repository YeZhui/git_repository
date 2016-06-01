#include "FileLog.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_FILE_LENGTH (50*1024*1024)

long GetCurTime()
{
	char szCurTime[10] = {0};
	long lCurTime = 0;
	time_t tCurTime = time(NULL);
	struct tm* stTm = localtime(&tCurTime);
	snprintf(szCurTime, sizeof(szCurTime)-1, "%04d%02d%02d", stTm->tm_year+1900, stTm->tm_mon+1, stTm->tm_mday);
	lCurTime = atol(szCurTime);
	return lCurTime;
}

CFileLog::CFileLog()
{
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
	m_iFileNum = 0;
	Init();
}

CFileLog::~CFileLog()
{
	Close();
}

void CFileLog::Init()
{
	char szAllPath[128] = {0};
	//配置文件读取日志文件存放路径 TODO:
	memset(m_szFilePath, 0, sizeof(m_szFilePath));
	strncpy(m_szFilePath, "./", sizeof(m_szFilePath)-1);
	//获取当前日期作为路径
	m_lCurTime = GetCurTime();
	snprintf(szAllPath, sizeof(szAllPath)-1, "%s/%ld", m_szFilePath, m_lCurTime );

	CheckDir(szAllPath);
	ReOpen();
}

void CFileLog::CheckDir(const char* p_szAllPath)
{
	char szFileName[64] = {0};
	char szFileNum[8] = {0};
	struct dirent * entry = NULL;
	DIR *dirptr = NULL;
	dirptr = opendir(p_szAllPath);
	if (dirptr == NULL)
	{
		mkdir(p_szAllPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		m_iFileNum = 0;
	}
	else
	{
		while( (entry = readdir(dirptr)) != NULL )
		{
			strncpy(szFileName, entry->d_name, sizeof(szFileName)-1);	
		}
		//根据文件名获取当前文件序号
		if (strstr(szFileName, "runlog") == NULL)
		{
			m_iFileNum = 0;
		}
		else
		{
			strncpy(szFileNum, &(szFileName[6]), sizeof(szFileNum)-1); 
			for(int i=0;i<8;i++)
			{
				if (szFileNum[i] == '.')
				{
					szFileNum[i] = 0;
					break;
				}
			}
			m_iFileNum = atoi(szFileNum);
		}
	}
	closedir(dirptr);
}

int CFileLog::ReOpen()
{
	char szFileName[256] = {0};
	Close();

	snprintf(szFileName, sizeof(szFileName)-1, "%s/%ld/runlog%d.log", m_szFilePath, m_lCurTime, m_iFileNum); 
	m_file.open(szFileName, ios::out|ios::app);
	return 0;
}

int CFileLog::CheckFile()
{
	char szFileName[256] = {0};
	long lCurTime = 0;
	int length = 0;
	
	//要检查日期，一旦日期发生变化写日志就要修改
	lCurTime = GetCurTime();	
	if (lCurTime > m_lCurTime)
	{
		m_lCurTime = lCurTime;
		m_iFileNum = 0;
		ReOpen();
	}
	else
	{
		//超过50M就写下一个文件
		length = m_file.tellg();
		if (length > MAX_FILE_LENGTH)
		{
			m_file.flush();
			m_iFileNum++;
			ReOpen();
		}
	}
	return 0;
}


int CFileLog::ReadAndPrint()
{
	string strLog;
	//int count = 0;
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
			CheckFile();
			m_file << strLog.c_str() << endl;
			//count++;
			//每写100条检查文件大小，文件刚打开时检查文件大小
			/*
			if (count == 100)
			{
				ReOpen();
				count = 0;
			}
			*/
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

