#ifndef __BASE_LOG_H_
#define __BASE_LOG_H_

#include <iostream>
#include <queue>
#include <string>
#include <pthread.h>
using namespace std;

class CBaseLog
{
public:
	CBaseLog();
	virtual ~CBaseLog();
	int         Write(const char* p_szLog);
	virtual int ReadAndPrint() = 0;

protected:
	queue<string>   m_queLog; 
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
};

void *ReadThread(void* arg);

#endif

