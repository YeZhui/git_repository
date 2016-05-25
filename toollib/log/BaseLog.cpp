#include "BaseLog.h"

CBaseLog::CBaseLog()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
}

CBaseLog::~CBaseLog()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
}

int CBaseLog::Write(const char* p_szLog)
{
	pthread_mutex_lock(&m_mutex);
	m_queLog.push(string(p_szLog));
	pthread_mutex_unlock(&m_mutex);
	pthread_cond_signal(&m_cond);
	return 0;
}

void *ReadThread(void* arg)
{
	pthread_detach(pthread_self());
	CBaseLog *baseLog = (CBaseLog*)arg;
	baseLog->ReadAndPrint();
}

