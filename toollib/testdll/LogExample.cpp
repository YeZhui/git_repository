#include "LogExample.h"
#include "../log/SingleLog.h"
#include <stdio.h>

int CheckSinglog()
{
	long lRet = 0;
	lRet = SingleLog::Init('P');
	if (lRet != 0)
	{
		printf("д��־��ʼ��ʧ��\n");
		return -1;
	}
	lRet = SingleLog::WriteLog(1,__FILE__,__LINE__, "%s", "��ʼִ��");
	if (lRet != 0)
	{
		printf("д��־ʧ��\n");
		return -1;
	}
	return 0;
}

