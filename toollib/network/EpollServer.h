#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "SxTcp.h"

//Tcp类
class CEpollServer
{

//构造函数
public:
    CEpollServer ();
    virtual ~CEpollServer ();

//公有成员函数
public:
    int CreateEpoll(const char* szIp, int nPort, int nSize);
    int ProcessEpoll();
    int CloseEpoll();

//私有成员变量
private:
	CTcp m_cTcp;
    int m_nEpollFd;
};

#endif


