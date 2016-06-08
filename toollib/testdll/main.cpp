#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "LogExample.h"
#include "En_decryptExample.h"
#include "../log/SingleLog.h"
#include "ReadIniExample.h"

int main(int argc, char* argv[])
{
	if (CheckSinglog() != 0)
		return -1;
//	CheckEnDecrypt();
	CheckReadIni();
	

	sleep(10);
	SingleLog::Delete();
	while(1)
	{
		sleep(5);
	}
	return 0;
}
