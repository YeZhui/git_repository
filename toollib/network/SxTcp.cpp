/*******************************************************************************
��    �ܣ�tcp������
˵    ����
��    �ߣ���Ҷ׷
�������ڣ�2018.07.23
�޸���־��
    ---------------------------------------------------------------------------
    ���ߣ���Ҷ׷
    ���ڣ�2018.07.23
    ���ݣ�����ʵ��tcpͨѶ��
    ���������δ���
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

//���캯��
CTcp::CTcp ()
{
    m_nSock = -1;
}

//���캯��
CTcp::CTcp (int p_iSock)
{
    m_nSock = p_iSock;
}

//��������
CTcp::~CTcp ()
{
    Close ();
}

/*��ֵ
  ��Σ�nSockfd - socket���
  ���Σ���ֵ���socket���
*/
int CTcp::operator = (int p_iSockfd)
{
	//isfdtype�ж�nSockfd�Ƿ�Ϊָ�����ͣ�S_IFSOCK�ж��Ƿ�Ϊ�׽���������������1�ǣ�0���ǣ�-1����
    assert ((m_nSock == -1) && (p_iSockfd > -1) && (isfdtype (p_iSockfd, S_IFSOCK) == 1));
    m_nSock = p_iSockfd;

    return m_nSock;
}

/*�ж�����socket����Ƿ����
  ��Σ�n - "!="�ұߵ�socket���
  ���Σ�1������ȣ�0�����
*/
int CTcp::operator != (int p_iSockfd) const
{
    return (m_nSock != p_iSockfd);
}

/*�ж�����socket����Ƿ����
  ��Σ�n - "=="�ұߵ�socket���
  ���Σ�1����ȣ�0�������
*/
int CTcp::operator == (int p_iSockfd) const
{
    return (m_nSock == p_iSockfd);
}

/*ȡ��socket���
  ��Σ���
  ���Σ�ȡ����socket���
*/
int CTcp::GetHandle () const
{
    return m_nSock;
}

/*����socket
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
*/
int CTcp::Open ()
{
    assert (m_nSock == -1);

	//��ȡtcp�׽���
    m_nSock = socket (AF_INET, SOCK_STREAM, 0);

    return (m_nSock != -1);
}

/*�ر�socket
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���ӣ�δ���ó�ʱ����Ĭ��Ϊ����IO
  ��Σ�pHost - IP��ַ��������
    nPort - �˿�
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*��
  ��Σ�pIP - IP��ַ
    nPort - �˿�
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*����
  ��Σ�nNum - ������Ŀ
  ���Σ�1: �ɹ� ; 0: ʧ��
*/
int CTcp::Listen (int nNum) const
{
    assert ((m_nSock != -1) && (nNum > 0));

    return (listen (m_nSock, nNum) == 0);
}

/*��������
  ��Σ���
  ���Σ�����: �����׽��־�� ; -1: ʧ��
*/
int CTcp::Accept () const
{
    assert (m_nSock != -1);

    return (accept (m_nSock, (struct sockaddr *)NULL, NULL));
}

/*����˽�������
  ��Σ�pBuf - ���ջ���
    nCount - ������ֽ���
  ���Σ�ʵ�ʽ����ֽ��� ���������ʧ�ܣ����ظ���������Է��رգ�����0
*/
int CTcp::Recv(int nFd, char* buf, int nBufLen)
{
	assert(nFd != -1 );
	return recv(nFd, buf, nBufLen, 0);
}

/*�ͻ��˽�������
  ��Σ�pBuf - ���ջ���
    nCount - ������ֽ���
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʽ����ֽ��� ���������ʧ�ܣ����ظ���������Է��رգ�����0
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



/*����˷�������
��Σ�pBuf - ���ͻ���
nCount - �跢���ֽ���
���Σ�ʵ�ʷ����ֽ��� ���������ʧ�ܣ����ظ���
*/
int CTcp::Send(int nFd, char* buf, int nBufLen)
{
	assert(nFd != -1 );
	return send(nFd, buf, nBufLen, 0);
}

/*�ͻ��˷�������
  ��Σ�pBuf - ���ͻ���
    nCount - �跢���ֽ���
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʷ����ֽ��� ���������ʧ�ܣ����ظ���
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


/*����ָ����Ŀ����
  ��Σ�pBuf - ���ͻ���
    nCount - �跢���ֽ���
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʷ����ֽ��� ���������ʧ�ܣ����ظ���
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



/*����ָ����Ŀ����
  ��Σ�pBuf - ���ջ���
    nCount - ������ֽ���
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʽ����ֽ��� ���������ʧ�ܣ����ظ���������Է��رգ�����0
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

/*����һ������(��"\r\n"��β)
  ��Σ�pBuf - ���ջ���
    nMaxCount - һ�е���󳤶�(����"\r\n")
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʽ����ֽ��� ���������ʧ�ܣ����ظ���������Է��رգ�����0
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

/*����һ������(��"\n"��β)
  ��Σ�pBuf - ���ջ���
    nMaxCount - һ�е���󳤶�(����"\n")
    nMicsec - socket��ʱֵ����λ��΢�ȱʡ��500000΢��
  ���Σ�ʵ�ʽ����ֽ��� ���������ʧ�ܣ����ظ���������Է��رգ�����0
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

/*�õ�����������
  ��Σ�pName - ��ű���������
  ���Σ�1: �ɹ� ; 0: ʧ��
*/
int CTcp::GetHostName (char *pName) const
{
    assert (pName);

    return (gethostname (pName, 100) == 0);
}

/*�õ��Է�������
  ��Σ�pName - ��ŶԷ�������
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*�õ�������ַ
  ��Σ���
  ���Σ���ַ�б������������NULL
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

/*�õ��Է���ַ
  ��Σ�pAddr - ��ŶԷ���ַ
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���ӣ����ó�ʱ��
  ��Σ�pHost - IP��ַ��������
    nPort - �˿�
    nSec - ��ʱֵ����λ���룩
  ���Σ�1: �ɹ� ; 0: ʧ��
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

    flags = fcntl (m_nSock, F_GETFL, 0);  //��ȡ�ļ�״̬
    fcntl (m_nSock, F_SETFL, flags | O_NONBLOCK);   //�����ļ�������״̬��ָ��Ϊ������IO

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


/*����socketΪ������
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*����socketΪ����
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���õ�ַ������
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���÷��ͳ�ʱֵ
  ��Σ����ͳ�ʱֵ����λ��΢�룩
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���ý��ճ�ʱֵ
  ��Σ����ճ�ʱֵ����λ���룩
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���÷��ͻ������Ĵ�С
  ��Σ����ͻ�����ֵ����λ���ֽڣ�
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���ý��ջ������Ĵ�С
  ��Σ����ջ�����ֵ����λ���ֽڣ�
  ���Σ�1: �ɹ� ; 0: ʧ��
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

/*���ô����
  ��Σ���
  ���Σ�1: �ɹ� ; 0: ʧ��
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


