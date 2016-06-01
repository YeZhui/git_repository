#include "SysLog.h"

CSysLog::CSysLog()
{
	Init();
}

CSysLog::~CSysLog()
{
	Close();
}

void CSysLog::Init()
{
	char szAddress[64] = {0};
	int  iPort = 0;
	Connect(szAddress, iPort);
}

int CSysLog::Connect(const char* p_szAddress, int p_iPort)
{
	return 0;
}

int CSysLog::Reconnect()
{
	return 0;
}

int CSysLog::ReadAndPrint()
{
	//如果连接断开重连
	Reconnect();
}

void CSysLog::Close()
{
}

