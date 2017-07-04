#include "En_decryptExample.h"
#include "../log/SingleLog.h"
#include "../en_decrypt/EnDecrypt.h"
#include <string.h>

int CheckEnDecrypt()
{
	long lRet = 0;
	char szStr[64] = "guanyezhui";
	char szDest[64] = {0};	
	char szDeBase64[128] = {0}; 
	/*
	lRet = En_Decrypt::Base64Encode(szStr, strlen(szStr), szDest, sizeof(szDest));
	if (lRet != 0)
	{
		SingleLog::WriteLog(2,__FILE__,__LINE__, "要求目标字符串长度为:%ld", lRet);
		return -1;
	}
	else
	{
		SingleLog::WriteLog(0,__FILE__,__LINE__, "源:%s,目标:%s", szStr, szDest);
	}

	lRet = En_Decrypt::Base64Decode(szDest, strlen(szDest), szDeBase64, sizeof(szDeBase64));
	if (lRet != 0)
	{
		SingleLog::WriteLog(2,__FILE__,__LINE__, "要求目标字符串长度为:%ld", lRet);
		return -1;
	}
	else
	{
		SingleLog::WriteLog(0,__FILE__,__LINE__, "源:%s,目标:%s", szDest, szDeBase64);
	}
	*/
	lRet = En_Decrypt::Md5Encrypt(szStr, strlen(szStr),szDest, sizeof(szDest));
	if (lRet != 0)
	{
	}
	else
	{
		SingleLog::WriteLog(0, __FILE__,__LINE__,"源:%s,目标:%s", szStr, szDest);
	}


	//解码
	return 0;
}

