#include "LogExample.h"
#include "../log/SingleLog.h"
#include <stdio.h>

int CheckSinglog()
{
	long lRet = 0;
	lRet = SingleLog::Init('P');
	if (lRet != 0)
	{
		printf("写日志初始化失败\n");
		return -1;
	}
	lRet = SingleLog::WriteLog(1,__FILE__,__LINE__, "%s", "开始执行");
	if (lRet != 0)
	{
		printf("写日志失败\n");
		return -1;
	}
	return 0;
}

