#include <stdarg.h>
#include <thread>
#include <string.h>

#include "SingleLog.h"
#include "BaseLog.h"
#include "FileLog.h"
#include "SysLog.h"
#include "../readini/ConfigIni.h"
#include "PrintLog.h"

namespace SingleLog
{

	static CBaseLog *baseLog = NULL;
	
	#define FILE_LOG 'f'
	#define SYS_LOG 's'
	#define PRINT_LOG 'p'

	int Init()
	{
		Delete();

		//从配置文件读取日志输出目标
		ConfigIni::Init(string(CONFIG_DIR));
		auto flag = ConfigIni::ReadCh( "FLAG", "PRINT_FLAG");
		if (flag == 0)
		{
			std::cout << "read PRINT_FLAG failed" << std::endl;
			return -1;
		}

		if (flag == FILE_LOG)
		{
			baseLog = new (std::nothrow) CFileLog();
		}
		else if(flag == PRINT_LOG)
		{
			baseLog = new (std::nothrow) CPrintLog();
		}
		else if(flag == SYS_LOG)
		{
			baseLog = new (std::nothrow) CSysLog();
		}
		else
		{
			std::cout << "日志类型不存在" << std::endl;
			return -1;
		}

		if ( baseLog == nullptr )
		{
			std::cout << "分配内存失败" << std::endl;
			return -1;
		}

		std::thread t_read(ReadThread, (void*)baseLog);
		t_read.detach();

		return 0;
	}

	int WriteLog(int p_iLevel, const char* p_szMsg, ...)
	{
		char       szLog[4096] = {0};
		char 	   szCurTime[64] = {0};
		char 	   szViewMsg[4096] = {0};
		char       szLevel[16] = {0};
		va_list    arg_ptr;
		time_t     tmCurTime = 0;
		struct tm *stCurTime = NULL;

		if ( baseLog == nullptr )
		{
			fprintf(stderr, "日志未初始化\n");
			return -1;
		}

		tmCurTime = time(nullptr);
		stCurTime = localtime(&tmCurTime);
		snprintf(szCurTime, sizeof(szCurTime)-1, "%04d-%02d-%02d %02d:%02d:%02d", stCurTime->tm_year + 1900, stCurTime->tm_mon + 1, stCurTime->tm_mday, 
			stCurTime->tm_hour, stCurTime->tm_min, stCurTime->tm_sec);

		va_start(arg_ptr, p_szMsg);
		vsprintf(szViewMsg, p_szMsg, arg_ptr);
		va_end(arg_ptr);

		switch(p_iLevel)
		{
			case INFO:
				strncpy(szLevel, "info", sizeof(szLevel)-1);
				break;
			case WARNING:
				strncpy(szLevel, "warning", sizeof(szLevel)-1);
				break;
			case ERROR:
				strncpy(szLevel, "error", sizeof(szLevel)-1);
				break;
			default:
				strncpy(szLevel, "unknown", sizeof(szLevel)-1);
				break;
		}

		snprintf(szLog, sizeof(szLog)-1, "[%s:%d][%s][%s][%s]", __FILE__, __LINE__, szCurTime, szLevel, szViewMsg);

		return baseLog->Write(szLog);
	}

	void Delete()
	{
		if (baseLog != nullptr)
		{
			delete baseLog;
			baseLog = nullptr;
		}
	}

}
