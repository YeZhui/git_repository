#include "../log/SingleLog.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	long lRet = 0;
	lRet = SingleLog::SingleLog_Init('F');
	if (lRet != 0)
	{
		printf("------\n");
		return -1;
	}
	if (SingleLog::SingleLog_WriteLog(2,__FILE__,__LINE__, "%s", "Ö´ÐÐ³É¹¦") != 0)
	{
		printf("************\n");
		return -1;
	}
	while(1)
	{
		sleep(5);
	}
	return 0;
}
