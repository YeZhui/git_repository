#include "EpollServer.h"
#include <sys/epoll.h>
#include "TypeError.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

CEpollServer::CEpollServer ()
{
	m_nEpollFd = -1;
}

CEpollServer::~CEpollServer ()
{
	CloseEpoll();
	m_cTcp.Close();
}


/*创建epoll句柄
  入参：
  szIp 服务器ip地址
  nPort 要绑定的端口
  nSize 要监听的文件描述符数量
  出参：1: 成功 ; 0: 失败
*/
int CEpollServer::CreateEpoll(const char* szIp, int nPort, int nSize)
{
	assert(szIp != nullptr);
	int iRet = 0;
	int size = (nSize > 0 ? nSize : DEFAULT_EPOLL_FD_NUM);
	
	iRet = m_cTcp.Open();
	if ( iRet ==  0 )
	{
		return SOCKET_ERROR;
	}

	iRet = m_cTcp.Bind(szIp, nPort);
	if ( iRet == 0 )
	{
		return BIND_ERROR;
	}

	iRet = m_cTcp.SetNoblock();
	if ( iRet == 0 )
	{
		return SETSOCKOPT_ERROR;
	}

	iRet = m_cTcp.Listen(nSize+1);//监听描述符数量要比epoll的多？
	if ( iRet == 0)
	{
		return LISTEN_ERROR;
	}

	if ( m_nEpollFd != -1 )
	{
		CloseEpoll();
	}

	m_nEpollFd = epoll_create(size);
	if ( m_nEpollFd == -1)
	{
		return EPOLL_CREATE_ERROR;
	}

	return 1;
}

/*处理epoll事件
  出参：1: 成功 ; 0: 失败
*/
int CEpollServer::ProcessEpoll()
{
	assert(m_nEpollFd != -1);
	int nFds = 0;
	int connFd = -1, readFd = -1, writeFd = -1;
	int n = 0, nSize = 0;
	int nListenFd = -1;
	char buf[MAX_READ_SIZE] = {0};
	struct sockaddr_in clientAddr;
	socklen_t clilen;
	struct epoll_event ev, events[20];
	memset((void*)&ev, 0, sizeof(ev));
	nListenFd = m_cTcp.GetHandle();
	ev.data.fd = nListenFd;
	ev.events = EPOLLIN|EPOLLET;
	if ( epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, nListenFd, &ev) == -1 )
	{
		return EPOLL_CTL_ERROR;
	}
	while(1)
	{
		n = 0;
		nSize = 0;
		nFds = epoll_wait(m_nEpollFd, events, 20, 500);
		for (int i = 0; i< nFds; ++i)
		{
			memset(buf, 0, MAX_READ_SIZE);
			if (events[i].data.fd == nListenFd )
			{
				while ( (connFd = accept(nListenFd, (sockaddr*)&clientAddr, &clilen)) > 0 )
				{
					m_cTcp.SetNoblock(connFd);  //ET模式需设置为非阻塞的
					ev.data.fd = connFd;
					ev.events = EPOLLIN|EPOLLET;
					if ( epoll_ctl(m_nEpollFd, EPOLL_CTL_ADD, connFd, &ev) == -1 )
					{
						return EPOLL_CTL_ERROR;
					}
				}
				if ( connFd == -1 && errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR )
				{
					return ACCEPT_ERROR;
				}
				continue;
			}
			else if(events[i].events & EPOLLIN)
			{
				readFd = events[i].data.fd;
				if (readFd < 0)
				{
					continue;
				}
				//读取数据
				while ( (nSize = read(readFd, buf+n, MAX_READ_SIZE - 1)) > 0 )
				{
					n += nSize;
				}
				//EAGAIN说明读到结尾了
				if (nSize == -1 && errno != EAGAIN )
				{
					fprintf(stderr, "epoll read failed\n");
					//ngleLog::WriteLog(ERROR, "%s", "epoll read fialed");
				}

				fprintf(stdout, "read data is:%s\n", buf);
				
				ev.data.fd = readFd;
				ev.events = EPOLLOUT|EPOLLET;//边沿模式(ET)
				epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, readFd, &ev);
			}
			else if(events[i].events & EPOLLOUT)
			{
				writeFd = events[i].data.fd;
				//写数据
				strncpy(buf, "hello client", sizeof(buf)-1);
				int dataSize = strlen(buf);
				n = dataSize;
				while(n > 0)
				{
					nSize = write(writeFd, buf + dataSize - n, n);
					if (nSize < n)
					{
						if (nSize == -1 && errno != EAGAIN)
						{							
							break;
						}
					}
					n -= nSize;
				}

				ev.data.fd = writeFd;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(m_nEpollFd, EPOLL_CTL_MOD, writeFd, &ev);
			}
		}
	}
}

/*
关闭epoll文件描述符
*/
int CEpollServer::CloseEpoll()
{
    if (m_nEpollFd != -1)
    {
        close (m_nEpollFd);
        m_nEpollFd = -1;
    }
    return 1;

}

