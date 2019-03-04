/*******************************************************************************
功    能：tcp操作类
说    明：
作    者：关叶追
创建日期：2018.07.23
修改日志：
    ---------------------------------------------------------------------------
    作者：关叶追
    日期：2018.07.23
    内容：初步实现tcp通讯类
    特征：初次创建
    ---------------------------------------------------------------------------

*******************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <assert.h>
#include <sys/epoll.h>
#include <signal.h>
#include "SxTcp.h"

const int CTcp::SOCK_ERROR = -100;
const int CTcp::SOCK_TIMEOUT = -101;
const long CTcp::TCP_TIMEOUT = 500000;

//构造函数
CTcp::CTcp ()
{
    m_nSock = -1;
}

//构造函数
CTcp::CTcp (int p_iSock)
{
    m_nSock = p_iSock;
}

//析构函数
CTcp::~CTcp ()
{
    Close ();
}

/*赋值
  入参：nSockfd - socket句柄
  出参：赋值后的socket句柄
*/
int CTcp::operator = (int p_iSockfd)
{
	//isfdtype判断nSockfd是否为指定类型，S_IFSOCK判断是否为套接字描述符，返回1是，0不是，-1出错
    assert ((m_nSock == -1) && (p_iSockfd > -1) && (isfdtype (p_iSockfd, S_IFSOCK) == 1));
    m_nSock = p_iSockfd;

    return m_nSock;
}

/*判断两个socket句柄是否不相等
  入参：n - "!="右边的socket句柄
  出参：1：不相等；0：相等
*/
int CTcp::operator != (int p_iSockfd) const
{
    return (m_nSock != p_iSockfd);
}

/*判断两个socket句柄是否相等
  入参：n - "=="右边的socket句柄
  出参：1：相等；0：不相等
*/
int CTcp::operator == (int p_iSockfd) const
{
    return (m_nSock == p_iSockfd);
}

/*取出socket句柄
  入参：无
  出参：取出的socket句柄
*/
int CTcp::GetHandle () const
{
    return m_nSock;
}

/*创建socket
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::Open ()
{
    assert (m_nSock == -1);

	//获取tcp套接字
    m_nSock = socket (AF_INET, SOCK_STREAM, 0);

    return (m_nSock != -1);
}

/*关闭socket
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::Close ()
{
    if (m_nSock != -1)
    {
        close (m_nSock);
        m_nSock = -1;
    }
    return 1;
}

/*连接（未设置超时），默认为阻塞IO
  入参：pHost - IP地址或主机名
    nPort - 端口
  出参：1: 成功 ; 0: 失败
*/
int CTcp::Connect (const char *p_szHost, int p_iPort) const
{
    assert ((m_nSock != -1) && p_szHost && (p_iPort > 0));
    struct sockaddr_in addr;
    struct hostent *phe = NULL;

    memset ((void*)&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons (p_iPort);

    if ((addr.sin_addr.s_addr = inet_addr (p_szHost)) == -1)
    {
        if ((phe = gethostbyname (p_szHost)) == NULL)
            return 0;

        memcpy ((char *)&addr.sin_addr, phe->h_addr, phe->h_length);
    }

    return (connect (m_nSock, (struct sockaddr *)&addr, sizeof (addr)) == 0);
}

/*绑定
  入参：pIP - IP地址
    nPort - 端口
  出参：1: 成功 ; 0: 失败
*/
int CTcp::Bind (const char *pIP, int nPort) const
{
    assert ((m_nSock != -1) && (nPort > 0));
    struct sockaddr_in addr;
    struct hostent *phe = NULL;
    int opt=1;

    if (setsockopt (m_nSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        return 0;
    }

    memset (&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons (nPort);

    if (!pIP)
    {

        addr.sin_addr.s_addr = htonl (INADDR_ANY);
    }
    else
    {
        if ((addr.sin_addr.s_addr = inet_addr (pIP)) == -1)
        {
            if ((phe = gethostbyname (pIP)) == NULL)
                return 0;

            memcpy ((char *)&addr.sin_addr, phe->h_addr, phe->h_length);
        }
    }

    return (bind (m_nSock, (struct sockaddr *)&addr, sizeof (addr)) == 0);
}

/*监听
  入参：nNum - 监听数目
  出参：1: 成功 ; 0: 失败
*/
int CTcp::Listen (int nNum) const
{
    assert ((m_nSock != -1) && (nNum > 0));

    return (listen (m_nSock, nNum) == 0);
}

/*接受连接
  入参：无
  出参：其他: 连接套接字句柄 ; -1: 失败
*/
int CTcp::Accept () const
{
    assert (m_nSock != -1);

    return (accept (m_nSock, (struct sockaddr *)NULL, NULL));
}

/*服务端接收数据
  入参：pBuf - 接收缓存
    nCount - 需接收字节数
  出参：实际接收字节数 ，如果接收失败，返回负数，如果对方关闭，返回0
*/
int CTcp::Recv(int nFd, char* buf, int nBufLen)
{
	assert(nFd != -1 );
	return recv(nFd, buf, nBufLen, 0);
}

/*客户端接收数据
  入参：pBuf - 接收缓存
    nCount - 需接收字节数
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际接收字节数 ，如果接收失败，返回负数，如果对方关闭，返回0
*/
int CTcp::Recv (void *pBuf, int nCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nCount > 0));
    int sn = 0, rn = 0;
    struct timeval tvlTime;
    fd_set rdfdset;

    if (nMicsec >= 0)
    {
        tvlTime.tv_sec = nMicsec / 1000000;
        tvlTime.tv_usec = abs (nMicsec - tvlTime.tv_sec * 1000000);
    }

    FD_ZERO (&rdfdset);
    FD_SET (m_nSock, &rdfdset);

    if (nMicsec > 0)
        sn = select (m_nSock + 1, &rdfdset, NULL, NULL, &tvlTime);
    else
        sn = select (m_nSock + 1, &rdfdset, NULL, NULL, NULL);

    switch (sn)
    {
    case -1:
        return SOCK_ERROR;

    case 0:
        return SOCK_TIMEOUT;
    }

    if ((rn = read (m_nSock, pBuf, nCount)) < 0)
        return SOCK_ERROR;

    return rn;
}



/*服务端发送数据
入参：pBuf - 发送缓存
nCount - 需发送字节数
出参：实际发送字节数 ，如果发送失败，返回负数
*/
int CTcp::Send(int nFd, char* buf, int nBufLen)
{
	assert(nFd != -1 );
	return send(nFd, buf, nBufLen, 0);
}

/*客户端发送数据
  入参：pBuf - 发送缓存
    nCount - 需发送字节数
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际发送字节数 ，如果发送失败，返回负数
*/
int CTcp::Send (const void *pBuf, int nCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nCount > 0));
    int sn = 0, wn = 0;
    struct timeval tvlTime;
    fd_set wtfdset;


    if (nMicsec >= 0)
    {
        tvlTime.tv_sec = nMicsec / 1000000;
        tvlTime.tv_usec = abs (nMicsec - tvlTime.tv_sec * 1000000);
    }

    FD_ZERO (&wtfdset);
    FD_SET (m_nSock, &wtfdset);

    if (nMicsec >= 0)
        sn = select (m_nSock + 1, NULL, &wtfdset, NULL, &tvlTime);
    else
        sn = select (m_nSock + 1, NULL, &wtfdset, NULL, NULL);

    switch (sn)
    {
    case -1:
        return SOCK_ERROR;

    case 0:
        return SOCK_TIMEOUT;
    }

    if ((wn = send (m_nSock, pBuf, nCount, 0)) <= 0)
        return SOCK_ERROR;

    return wn;
}


void CTcp::Close (int nFd)
{
	if (nFd != -1 )
	{
		close(nFd);
		nFd = -1;
	}
}


/*发送指定数目数据
  入参：pBuf - 发送缓存
    nCount - 需发送字节数
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际发送字节数 ，如果发送失败，返回负数
*/
int CTcp::Sendn (const void *pBuf, int nCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nCount > 0));
    int nLeft = nCount, nWritten = 0, sn = 0;
    const char *ptr = (const char *)pBuf;
    struct timeval tvlTime;
    fd_set wtfdset;


    if (nMicsec >= 0)
    {
        tvlTime.tv_sec = nMicsec / 1000000;
        tvlTime.tv_usec = abs (nMicsec - tvlTime.tv_sec * 1000000);
    }

    FD_ZERO (&wtfdset);
    FD_SET (m_nSock, &wtfdset);

    if (nMicsec > 0)
        sn = select (m_nSock + 1, NULL, &wtfdset, NULL, &tvlTime);
    else
        sn = select (m_nSock + 1, NULL, &wtfdset, NULL, NULL);

    switch (sn)
    {
    case -1:
        return SOCK_ERROR;

    case 0:
        return SOCK_TIMEOUT;

    default:
        while (nLeft > 0)
        {
            if ((nWritten = send (m_nSock, ptr, nLeft, 0)) <= 0)
            {
                if (errno == EINTR || errno == EPIPE)
                    return 0;
                else
                {
                    return SOCK_ERROR;
                }
            }

            nLeft -= nWritten;
            ptr += nWritten;
        }

        break;
    }

    return nCount;
}



/*接收指定数目数据
  入参：pBuf - 接收缓存
    nCount - 需接收字节数
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际接收字节数 ，如果接收失败，返回负数，如果对方关闭，返回0
*/
int CTcp::Recvn (void *pBuf, int nCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nCount > 0));
    int nLeft = nCount, nRead = 0, sn = 0;
    char *ptr = (char *)pBuf;
    struct timeval tvlTime;
    fd_set rdfdset;

    if (nMicsec >= 0)
    {
        tvlTime.tv_sec = nMicsec / 1000000;
        tvlTime.tv_usec = abs (nMicsec - tvlTime.tv_sec * 1000000);
    }

    FD_ZERO (&rdfdset);
    FD_SET (m_nSock, &rdfdset);

    if (nMicsec > 0)
        sn = select (m_nSock + 1, &rdfdset, NULL, NULL, &tvlTime);
    else
        sn = select (m_nSock + 1, &rdfdset, NULL, NULL, NULL);

    switch (sn)
    {
    case -1:
        return SOCK_ERROR;

    case 0:
        return SOCK_TIMEOUT;

    default:
        while (nLeft > 0)
        {
            if ((nRead = read (m_nSock, ptr, nLeft)) < 0)
            {
                if (errno == EINTR || errno == EPIPE)
                    return 0;
                else
                    return SOCK_ERROR;
            }
            else if (nRead == 0)
            {
                return 0;
            }

            nLeft -= nRead;
            ptr += nRead;
        }

        break;
    }

    return nCount;
}

/*接收一行数据(以"\r\n"结尾)
  入参：pBuf - 接收缓存
    nMaxCount - 一行的最大长度(包括"\r\n")
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际接收字节数 ，如果接收失败，返回负数，如果对方关闭，返回0
*/
int CTcp::RecvLine_rn (void *pBuf, int nMaxCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nMaxCount > 0));
    char *ptr = (char *)pBuf;
    int rn = 0, ret = 0;

    while (rn < nMaxCount)
    {
        if ((ret = Recvn (ptr + rn, 1, nMicsec)) <= 0)
            return ret;

        rn++;
        if ((rn >= 2) && (ptr[rn - 2] == '\r') && (ptr[rn - 1] == '\n'))
            break;
    }

    return rn;
}

/*接收一行数据(以"\n"结尾)
  入参：pBuf - 接收缓存
    nMaxCount - 一行的最大长度(包括"\n")
    nMicsec - socket超时值，单位：微妙，缺省：500000微妙
  出参：实际接收字节数 ，如果接收失败，返回负数，如果对方关闭，返回0
*/
int CTcp::RecvLine_n (void *pBuf, int nMaxCount, int nMicsec) const
{
    assert ((m_nSock != -1) && pBuf && (nMaxCount > 0));
    char *ptr = (char *)pBuf;
    int rn = 0, ret = 0;

    while (rn < nMaxCount)
    {
        if ((ret = Recvn (ptr + rn, 1, nMicsec)) <= 0)
            return ret;

        rn++;
        if ((rn >= 1) && (ptr[rn - 1] == '\n'))
            break;
    }

    return rn;
}

/*得到本机机器名
  入参：pName - 存放本机机器名
  出参：1: 成功 ; 0: 失败
*/
int CTcp::GetHostName (char *pName) const
{
    assert (pName);

    return (gethostname (pName, 100) == 0);
}

/*得到对方机器名
  入参：pName - 存放对方机器名
  出参：1: 成功 ; 0: 失败
*/
int CTcp::GetPeerName (char *pName) const
{
    assert ((m_nSock != -1) && pName);
    struct hostent *pht = NULL;
    struct sockaddr_in addr;
    socklen_t len;

    if (getpeername (m_nSock, (struct sockaddr *)&addr, &len) == -1)
        return 0;

    if ((pht = gethostbyaddr (&addr.sin_addr, sizeof (addr.sin_addr), AF_INET)) != NULL)
    {
        strcpy (pName, pht->h_name);
        return 1;
    }

    return 0;
}

/*得到本机地址
  入参：无
  出参：地址列表，如果出错，返回NULL
*/
char **CTcp::GetHostAddr () const
{
//  char pName[100], **pAddr = NULL;
//  struct hostent *pht = NULL;
//
//  if (!GetHostName (pName))
//      return NULL;
//
//  if ((pht = gethostbyname (pName)) != NULL)
//  {
//      int i = 0;
//      char **pList = pht->h_addr_list;
//
//      for(; *pList != NULL; pList++)
//      {
//          pAddr = (char **) mrealloc (pAddr, (i + 2) * sizeof (char *));
//          pAddr[i] = (char *) mmalloc(100);
//          pAddr[i+1] = NULL;
//
//          if (inet_ntop (AF_INET, *pList, pAddr[i], 100) == NULL)
//          {
//              strfreev (pAddr);
//              return NULL;
//          }
//
//          i++;
//      }
//
//      return pAddr;
//  }

    return NULL;
}

/*得到对方地址
  入参：pAddr - 存放对方地址
  出参：1: 成功 ; 0: 失败
*/
int CTcp::GetPeerAddr (int sock,char *pAddr) const
{
    assert ((m_nSock != -1) && pAddr);
    struct sockaddr_in addr;
    socklen_t len = 16;

    if (getpeername (sock, (struct sockaddr *)&addr, &len) == -1)
        return 0;

    return (inet_ntop (AF_INET, &addr.sin_addr, pAddr, 16) != NULL);
}

/*连接（设置超时）
  入参：pHost - IP地址或主机名
    nPort - 端口
    nSec - 超时值（单位：秒）
  出参：1: 成功 ; 0: 失败
*/
int CTcp::ConnectNoblock (const char *p_szHost, int p_iPort, int p_iSec) const
{
    assert ((m_nSock != -1) && p_szHost && (p_iPort > 0));

    int    flags = 0;
    struct timeval TimeOut, *pTimeOut = NULL;
    fd_set readfds, writefds;
    int    nRetVal = 0;
    int    nerror = 0;
    int    len = 0;

	memset((void*)&TimeOut, 0, sizeof(TimeOut));

    flags = fcntl (m_nSock, F_GETFL, 0);  //读取文件状态
    fcntl (m_nSock, F_SETFL, flags | O_NONBLOCK);   //设置文件描述符状态，指定为非阻塞IO

    nerror = 0;
    if (p_iSec < 0) 
    	pTimeOut = NULL; 
    else  
    	pTimeOut = &TimeOut;
   
    if ((nRetVal = Connect (p_szHost, p_iPort)) == 0)
    {
        if(errno != EINPROGRESS)
        {
            return 0;
        }
    }
    if (nRetVal == 1)
    {
        goto Ok;
    }

    FD_ZERO (&readfds);
    FD_SET (m_nSock, &readfds);
    writefds = readfds;
    TimeOut.tv_sec = p_iSec;
    TimeOut.tv_usec=0;

test:
    if ((nRetVal = select (m_nSock + 1,&readfds, &writefds, NULL, pTimeOut)) == 0)
    {   //timeout
        return 0;
    }
    else if ((nRetVal < 0) && (errno == EINTR || errno == EPIPE))
        goto test;//interrupt by signal

    if (FD_ISSET (m_nSock, &readfds) || FD_ISSET (m_nSock, &writefds))
    {
        len = sizeof (nerror);
        if (getsockopt (m_nSock, SOL_SOCKET, SO_ERROR, &nerror, (socklen_t *)&len) < 0)
            return  0;
    }
    else return 0;

Ok:
    fcntl (m_nSock, F_SETFL, flags);
    if (nerror) return 0;

    return 1;
}


/*设置socket为非阻塞
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetNoblock (int nSock)
{
    assert (m_nSock != -1);
    int nFlags;

    if ( nSock == -1 )
    {
    	nSock = m_nSock;
    }

    if ((nFlags = fcntl (nSock, F_GETFL, 0)) < 0)
        return 0;

    nFlags = nFlags | O_NONBLOCK;

    if (fcntl (nSock, F_SETFL, nFlags) < 0)
        return 0;

    return 1;
}

/*设置socket为阻塞
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetBlock ()
{
    assert (m_nSock != -1);
    int nFlags;

    if ((nFlags = fcntl (m_nSock, F_GETFL, 0)) < 0)
        return 0;

    nFlags = nFlags & (~O_NONBLOCK);

    if (fcntl (m_nSock, F_SETFL, nFlags) < 0)
        return 0;

    return 1;
}

/*设置地址可重用
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetReuseAddr ()
{
    assert (m_nSock != -1);
    int opt = 1;

    if (setsockopt (m_nSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
    {
        return 0;
    }

    return 1;
}

/*设置发送超时值
  入参：发送超时值（单位：微秒）
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetSendTimeout (int nTimeout)
{
    assert ((m_nSock != -1) && (nTimeout > 0));
    struct  timeval tv;

    tv.tv_sec = nTimeout / 1000000;
    tv.tv_usec = abs (nTimeout - tv.tv_sec * 1000000);

    if (setsockopt (m_nSock, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1)
    {
        return 0;
    }

    return 1;
}

/*设置接收超时值
  入参：接收超时值（单位：秒）
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetRecvTimeout (int nTimeout)
{
    assert ((m_nSock != -1) && (nTimeout > 0));
    struct  timeval tv;

    tv.tv_sec = nTimeout / 1000000;
    tv.tv_usec = abs (nTimeout - tv.tv_sec * 1000000);

    if (setsockopt (m_nSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
    {
        return 0;
    }

    return 1;
}

/*设置发送缓冲区的大小
  入参：发送缓冲区值（单位：字节）
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetSendBuf (int nSendBuf)
{
    assert ((m_nSock != -1) && (nSendBuf > 0));
    int opt = nSendBuf;

    if (setsockopt (m_nSock, SOL_SOCKET, SO_SNDBUF, &opt, sizeof(int)) == -1)
    {
        return 0;
    }

    return 1;
}

/*设置接收缓冲区的大小
  入参：接收缓冲区值（单位：字节）
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetRecvBuf (int nRcvBuf)
{
    assert ((m_nSock != -1) && (nRcvBuf > 0));
    int opt = nRcvBuf;

    if (setsockopt (m_nSock, SOL_SOCKET, SO_RCVBUF, &opt, sizeof(int)) == -1)
    {
        return 0;
    }

    return 1;
}

/*设置存活检测
  入参：无
  出参：1: 成功 ; 0: 失败
*/
int CTcp::SetKeepalive ()
{
    assert (m_nSock != -1);
    int opt = 1;

    if (setsockopt (m_nSock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int)) == -1)
    {
        return 0;
    }

    return 1;
}

void CTcp::SignalHandler()
{
	sigset_t sigSet;
	sigemptyset(&sigSet);
	sigaddset(&sigSet, SIGINT);
	sigaddset(&sigSet, SIGQUIT);
	sigprocmask(SIG_BLOCK, &sigSet, NULL);
}


