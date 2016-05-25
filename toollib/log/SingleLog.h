#ifndef __SINGLE_LOG_H_
#define __SINGLE_LOG_H

namespace SingleLog
{
	int  SingleLog_Init(const char p_cFileType);
	int  SingleLog_WriteLog(int p_iLevel, const char* p_szFileName, int p_iLine, const char* p_szMsg, ...);
	void SingleLog_Delete();
		
}

#endif

