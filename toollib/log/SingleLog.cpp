#include <stdarg.h>

#include "SingleLog.h"
#include "BaseLog.h"
#include "FileLog.h"
#include "SysLog.h"
#include "PrintLog.h"

namespace SingleLog
{

	static CBaseLog *baseLog = NULL;

	int Init(const char p_cFileType)
	{
		pthread_t t_read;
		int iRet = 0;

		Delete();

		if (p_cFileType == 'F' || p_cFileType == 'f')
		{
			baseLog = new CFileLog();
		}
		else if(p_cFileType == 'P' || p_cFileType == 'p')
		{
			baseLog = new CPrintLog();
		}
		else if(p_cFileType == 'S' || p_cFileType == 's')
		{
			baseLog = new CSysLog();
		}
		else
		{
			fprintf(stderr, "日志类型不存在\n");
			return -1;
		}

		iRet = pthread_create(&t_read, NULL, ReadThread, (void*)baseLog);
		if (iRet != 0)
		{
			fprintf(stderr, "线程创建失败\n");
			return -1;
		}

		return 0;
	}

	int WriteLog(int p_iLevel, const char* p_szFileName, int p_iLine, const char* p_szMsg, ...)
	{
		char       szLog[4096] = {0};
		char 	   szCurTime[64] = {0};
		char 	   szViewMsg[4096] = {0};
		char       szLevel[16] = {0};
		va_list    arg_ptr;
		time_t     tmCurTime = 0;
		struct tm *stCurTime = NULL;

		tmCurTime = time(NULL);
		stCurTime = localtime(&tmCurTime);
		snprintf(szCurTime, sizeof(szCurTime)-1, "%04d-%02d-%02d %02d:%02d:%02d", stCurTime->tm_year + 1900, stCurTime->tm_mon + 1, stCurTime->tm_mday, 
			stCurTime->tm_hour, stCurTime->tm_min, stCurTime->tm_sec);

		va_start(arg_ptr, p_szMsg);
		vsprintf(szViewMsg, p_szMsg, arg_ptr);
		va_end(arg_ptr);

		switch(p_iLevel)
		{
			case 0:
				strncpy(szLevel, "info", sizeof(szLevel)-1);
				break;
			case 1:
				strncpy(szLevel, "warning", sizeof(szLevel)-1);
				break;
			case 2:
				strncpy(szLevel, "error", sizeof(szLevel)-1);
				break;
			default:
				strncpy(szLevel, "unknown", sizeof(szLevel)-1);
				break;
		}

		snprintf(szLog, sizeof(szLog)-1, "[%s:%d][%s][%s][%s]", p_szFileName, p_iLine, szCurTime, szLevel, szViewMsg);

		return baseLog->Write(szLog);
	}

	void Delete()
	{
		if (baseLog != NULL)
		{
			delete baseLog;
			baseLog = NULL;
		}
	}

}
