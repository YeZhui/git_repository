#include "../../log/SingleLog.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
	long lRet = 0;
	lRet = SingleLog::Init();
	if (lRet != 0)
	{
		fprintf(stderr, "log init failed\n");
		return -1;
	}
	while(1)
	{
		lRet = SingleLog::WriteLog(INFO, "%s", "start wirte log");
		if (lRet != 0)
		{
			fprintf(stderr, "write log failed\n");
			return -1;
		}
		usleep(100);
	}


	while(1)
	{
		sleep(1);
	}

	return 0;
}

