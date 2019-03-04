#ifndef __SINGLE_LOG_H_
#define __SINGLE_LOG_H

enum
{
	INFO = 0,
	WARNING = 1,
	ERROR = 2,
	UNKNOWN = 3
};

namespace SingleLog
{
	//注意：在主进程里面一定要调用SingleLog::Init()函数
	int  Init();
	int  WriteLog(int p_iLevel, const char* p_szMsg, ...);
	void Delete();
		
}

#endif

