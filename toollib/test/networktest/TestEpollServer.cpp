#include "../../readini/ConfigIni.h"
#include <string>
#include "../../network/EpollServer.h"
#include "../../log/SingleLog.h"

CEpollServer g_clEpollServer;
#define FILEDIR "./socket.ini"

//epoll server
int epoll_server_init()
{	
	int iRet = -1;
	string strIp;
	int nPort = 0, nEpollNum = 0, nTimeout = 0;
	ConfigIni::Init(string(FILEDIR));
	strIp = ConfigIni::ReadStr(string("SERVER"), string("Addr"));
	if (strIp == "")
	{
		SingleLog::WriteLog(ERROR,"read server addr failed");
		return iRet;
	}

	nPort = ConfigIni::ReadInt(string("SERVER"), string("Port"));
	if ( nPort == -1 )
	{
		SingleLog::WriteLog(ERROR,"read server port failed");
		return iRet;
	}

	nEpollNum = ConfigIni::ReadInt(string("SERVER"), string("MaxEpollNum"));
	if ( nEpollNum == -1 )
	{
		SingleLog::WriteLog(ERROR,"read server epoll num failed");
		return iRet;
	}

	nTimeout = ConfigIni::ReadInt(string("SERVER"), string("Timeout"));
	if ( nTimeout == -1 )
	{
		SingleLog::WriteLog(ERROR,"read server timeout failed");
		return iRet;
	}

	iRet = g_clEpollServer.CreateEpoll(strIp.c_str(), nPort, nEpollNum);
	if ( iRet == 0 )
	{
		SingleLog::WriteLog(ERROR, "epoll create failed");
		return -1;
	}
	
	return 0;
}

void epoll_server_run()
{
	g_clEpollServer.ProcessEpoll();
}

int main()
{
	SingleLog::Init();
	if (epoll_server_init() == -1)
	{
		return -1;
	}
	epoll_server_run();
	return 0;
}
	
