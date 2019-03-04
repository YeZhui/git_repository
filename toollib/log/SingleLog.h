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
	//ע�⣺������������һ��Ҫ����SingleLog::Init()����
	int  Init();
	int  WriteLog(int p_iLevel, const char* p_szMsg, ...);
	void Delete();
		
}

#endif

