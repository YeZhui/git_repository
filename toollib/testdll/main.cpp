#include "../log/SingleLog.h"
#include "../en_decrypt/EnDecrypt.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int CheckSinglog()
{
	long lRet = 0;
	lRet = SingleLog::Init('F');
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

int CheckEnDecrypt()
{
	long lRet = 0;
	char szStr[64] = "guanyezhui";
	char szDest[64] = {0};	
	char szDeBase64[128] = {0}; 
	lRet = En_Decrypt::Base64Encode(szStr, strlen(szStr), szDest, sizeof(szDest));
	if (lRet != 0)
	{
		SingleLog::WriteLog(2,__FILE__,__LINE__, "Ҫ��Ŀ���ַ�������Ϊ:%ld", lRet);
		return -1;
	}
	else
	{
		SingleLog::WriteLog(0,__FILE__,__LINE__, "Դ:%s,Ŀ��:%s", szStr, szDest);
	}

	lRet = En_Decrypt::Base64Decode(szDest, strlen(szDest), szDeBase64, sizeof(szDeBase64));
	if (lRet != 0)
	{
		SingleLog::WriteLog(2,__FILE__,__LINE__, "Ҫ��Ŀ���ַ�������Ϊ:%ld", lRet);
		return -1;
	}
	else
	{
		SingleLog::WriteLog(0,__FILE__,__LINE__, "Դ:%s,Ŀ��:%s", szDest, szDeBase64);
	}


	//����
	return 0;
}

int main(int argc, char* argv[])
{
	if (CheckSinglog() != 0)
		return -1;
	CheckEnDecrypt();
	sleep(10);
	SingleLog::Delete();
	while(1)
	{
		sleep(5);
	}
	return 0;
}
