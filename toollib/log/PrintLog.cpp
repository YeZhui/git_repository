#include "PrintLog.h"

CPrintLog::CPrintLog()
{
	Init();
}

CPrintLog::~CPrintLog()
{
	Close();
}

void CPrintLog::Init()
{
}

int CPrintLog::ReadAndPrint()
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
			cout << strLog << endl;
		}
		pthread_mutex_unlock(&m_mutex);
	}
	return 0;
}

void CPrintLog::Close()
{
}

