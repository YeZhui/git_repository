#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include "SysLog.h"
#include "../readini/ConfigIni.h"

CSysLog::CSysLog()
{
	Init();
}

CSysLog::~CSysLog()
{
	Close();
}

void CSysLog::Init()
{
	int  iPort = ConfigIni::ReadInt("SYSLOG", "PORT");
	if ( iPort == -1 )
	{
		cout << "read PORT failed" << endl;
		exit(-1);
	}
	auto str = ConfigIni::ReadStr("SYSLOG", "ADDRESS");
	if ( str == "" )
	{
		cout << "read ADDRESS failed" << endl;
		exit(-1);
	}
	if (Connect(str.c_str(), iPort) != 0)
	{
		cout << "connect syslog failed" << endl;
		exit(-1);
	}
}

int CSysLog::Connect(const char* p_szAddress, int p_iPort)
{
	char szProcDir[128] = {0};
	char szProcName[32] = {0};
	char *pEndPath = NULL;
	//得到程序名
	if (readlink("/proc/self/exe", szProcDir, sizeof(szProcDir)) <= 0 )
	{
		return -1;
	}
	pEndPath = strrchr( szProcDir, '/');
	if (pEndPath == NULL)
	{
		return -1;
	}

	++pEndPath;

	strcpy(szProcName, pEndPath);

	openlog(szProcName, LOG_PID, LOG_USER);
	return 0;
}

int CSysLog::Reconnect()
{
	return 0;
}

int CSysLog::ReadAndPrint()
{
	string strLog;
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
			syslog(LOG_INFO, strLog.c_str());	
		}
		pthread_mutex_unlock(&m_mutex);
	}
	return 0;
}

void CSysLog::Close()
{
	closelog();
}

