#ifndef __SYS_LOG_H_
#define __SYS_LOG_H_

#include "BaseLog.h"

class CSysLog : public CBaseLog
{
public:
	CSysLog();
	virtual ~CSysLog();
	virtual int ReadAndPrint();

private:
	void Init();
	int Connect(const char* p_szAddress, int p_iPort);
	int Reconnect();
	void Close();
};

#endif

