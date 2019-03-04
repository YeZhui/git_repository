#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "SxTcp.h"

//Tcp��
class CEpollServer
{

//���캯��
public:
    CEpollServer ();
    virtual ~CEpollServer ();

//���г�Ա����
public:
    int CreateEpoll(const char* szIp, int nPort, int nSize);
    int ProcessEpoll();
    int CloseEpoll();

//˽�г�Ա����
private:
	CTcp m_cTcp;
    int m_nEpollFd;
};

#endif


