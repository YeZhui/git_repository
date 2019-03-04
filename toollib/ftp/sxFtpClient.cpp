#include "sxFtpClient.h"
#include <unistd.h>
#include<stdlib.h>
#include<memory.h>
#include <netdb.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//构造函数
yCFtp::yCFtp ()
{
	FTPLIB_DIR = 1;
	FTPLIB_DIR_VERBOSE = 2;
	FTPLIB_FILE_READ = 3;
	FTPLIB_FILE_WRITE = 4;

	FTPLIB_ASCII = 'A';
	FTPLIB_IMAGE = 'I';
	FTPLIB_TEXT = FTPLIB_ASCII;
	FTPLIB_BINARY = FTPLIB_IMAGE;

	FTPLIB_PASSIVE = 1;
	FTPLIB_PORT = 2;

	FTPLIB_CONNMODE = 1;
	FTPLIB_CALLBACK = 2;
	FTPLIB_IDLETIME = 3;
	FTPLIB_CALLBACKARG = 4;
	FTPLIB_CALLBACKBYTES = 5;

	FTPLIB_BUFSIZ = 8192;
	ACCEPT_TIMEOUT = 5;
	FTPLIB_CONTROL = 0;
	FTPLIB_READ = 1;
	FTPLIB_WRITE = 2;
	FTPLIB_DEFMODE = FTPLIB_PASSIVE;

	m_pCtl = NULL;
}

//析构函数
yCFtp::~yCFtp ()
{
	Empty ();
}

/*连接Ftp服务器
  入参：pHost - 服务器IP（:端口）或机器名
  出参：成功返回1，失败返回0
  用法：Connect("192.168.0.20") 或
  	Connect("192.168.0.20:21")或
  	Connect("Ftpserver")或
  	Connect("Ftpserver:21")
*/
int yCFtp::Connect (const char *pHost, int nsec)	//增加连接超时nsec后返回错误值,为保持以前该方法的代码的可用性，这里设置超时默认值为5秒 Z.T.T @20051013
{
//	assert ((!m_pCtl) && pHost);	//此处若不屏蔽，则当以错误的用户名和密码第二次登陆FTP服务器时会产生错误，故将此屏蔽
	int nCtl = -1,on = 1;
	struct sockaddr_in sin;
	struct hostent *phe = NULL;
	struct servent *pse = NULL;
	char *plHost = strdup (pHost), *pNum = NULL;

//------增加连接超时----------	Z.T.T @20051013
	int    flags = 0;	
	struct timeval TimeOut,*pTimeOut=NULL;
	fd_set readfds,writefds;
	int    nRetVal = 0;
	int    nerror = 0;
	int    len = 0;
//------增加连接超时----------

	memset (&sin, 0, sizeof (sin));
	sin.sin_family = AF_INET;

	if ((pNum = strchr (plHost, ':')) == NULL)
	{
		if ((pse = getservbyname ("ftp", "tcp")) == NULL)
			return 0;

		sin.sin_port = pse->s_port;
	}
	else
	{
		*pNum++ = '\0';

		if (isdigit (*pNum))
			sin.sin_port = htons (atoi (pNum));
		else
		{
			if ((pse = getservbyname (pNum, "tcp")) == NULL)
				return 0;
			sin.sin_port = pse->s_port;
		}
	}

	if ((sin.sin_addr.s_addr = inet_addr (plHost)) == (unsigned  int) -1)
	{
		if ((phe = gethostbyname (plHost)) == NULL)
			return 0;

		memcpy ((char *)&sin.sin_addr, phe->h_addr, phe->h_length);
	}

	free (plHost);

	if ((nCtl = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return 0;

	if (setsockopt (nCtl, SOL_SOCKET, SO_REUSEADDR,
		  (void *)&on, sizeof (on)) == -1)
	{
		close (nCtl);
		nCtl = -1;			//Z.T.T&C.J.C@20050926
		return 0;
	}

//------增加连接超时----------	Z.T.T @20051013
	flags = fcntl(nCtl, F_GETFL, 0);
	fcntl(nCtl, F_SETFL, flags|O_NONBLOCK);
	nerror = 0; 
	if(nsec<0)
		pTimeOut = NULL;
	else
		pTimeOut = &TimeOut;
//------增加连接超时----------

	if ((nRetVal = connect(nCtl, (struct sockaddr *)&sin, sizeof (sin))) == -1)
	{

//------增加连接超时----------	Z.T.T @20051013	
		if(errno!=EINPROGRESS)
		{
			close (nCtl);
			nCtl = -1;
			return 0;	
		}
	}
	
	if(nRetVal==0)
	{	//connect ok
		goto Ok;
	}
	
	FD_ZERO(&readfds);
	FD_SET(nCtl,&readfds);
	writefds=readfds;
	
	TimeOut.tv_sec=nsec;//seconds
	TimeOut.tv_usec=0;

	test:	
		if((nRetVal=select(nCtl+1,&readfds,&writefds,NULL,pTimeOut)) == 0)
		{	//timeout
			return 0;
			/*return E_CONNECT_TIMEOUT; */
		}
		else if(nRetVal < 0 && errno == EINTR)
			goto test;//interrupt by signal
			
		if(FD_ISSET(nCtl,&readfds)||FD_ISSET(nCtl,&writefds))
		{
			len=sizeof(nerror);
			if(getsockopt(nCtl,SOL_SOCKET,SO_ERROR,&nerror, (socklen_t *)&len) < 0)
			{
				close (nCtl);
				nCtl = -1;
				return  0;
			}
		}

Ok:	
	fcntl(nCtl, F_SETFL, flags);
	if(nerror)
	{
		errno=nerror;
		return	0;
	}
//------增加连接超时----------

	//m_pCtl = (netbuf *) mmalloc (sizeof (netbuf));
	m_pCtl = (netbuf *) malloc (sizeof (netbuf));
	memset(m_pCtl, 0, sizeof (netbuf));			//Z.T.T&C.J.C@20050926
	//m_pCtl->pBuf = (char *) mmalloc (FTPLIB_BUFSIZ);
	m_pCtl->pBuf = (char *) malloc (FTPLIB_BUFSIZ);
	memset(m_pCtl->pBuf, 0, FTPLIB_BUFSIZ);		//Z.T.T&C.J.C@20050926
	
	m_pCtl->nHandle = nCtl;
	m_pCtl->nDir = FTPLIB_CONTROL;
	m_pCtl->pCtrl = NULL;
	m_pCtl->nMode = FTPLIB_DEFMODE;
	m_pCtl->funIdlecb = NULL;
	m_pCtl->stIdletime.tv_sec = m_pCtl->stIdletime.tv_usec = 0;
	m_pCtl->pIdleary = NULL;
	m_pCtl->nFered = 0;
	m_pCtl->nFeredl = 0;
	m_pCtl->nBytes = 0;

	if (!Readresp ('2', m_pCtl))
	{
		Empty ();
		return 0;
	}

	return 1;
}

/*登录Ftp服务器
  入参：pUser - 登录用户名
  	pPassword - 登录密码
  出参：成功返回1，失败返回0
*/
int yCFtp::Login (const char *pUser, const char *pPassword)
{
	assert (m_pCtl && pUser && pPassword);
	char pTempbuf[64];

	if (((strlen (pUser) + 7) > sizeof (pTempbuf)) ||
	   ((strlen (pPassword) + 7) > sizeof (pTempbuf)))
		return 0;

	sprintf (pTempbuf, "USER %s", pUser);

	if (!Sendcmd (pTempbuf, '3'))
	{
		if (m_pCtl->pResponse[0] == '2')
			return 1;

		return 0;
	}

	sprintf (pTempbuf, "PASS %s", pPassword);

	return Sendcmd (pTempbuf, '2');
}

/*连接并登录Ftp服务器
  入参：pHost - 服务器IP（:端口）或机器名
  	pUser - 登录用户名
  	pPassword - 登录密码
  出参：成功返回1，失败返回0
*/
int yCFtp::Connectlogin (const char *pHost, const char *pUser, const char *pPassword)
{
	assert (pHost && pUser && pPassword);

	if (!Connect(pHost))
		return 0;

	return Login(pUser, pPassword);	
}

/*执行"site"命令
  入参：pCmd - site 命令
  出参：成功返回1，失败返回0
*/
int yCFtp::Site (const char *pCmd)
{
	assert (m_pCtl && pCmd);
	char pBuf[256];

	if ((strlen (pCmd) + 7) > sizeof (pBuf))
		return 0;

	sprintf (pBuf, "SITE %s", pCmd);

	return (Sendcmd (pBuf, '2'));
}

/*执行"mkdir"命令
  入参：pPath - 要创建的目录名（相对或绝对路径都可）
  出参：成功返回1，失败返回0
*/
int yCFtp::Mkdir (const char *pPath)
{
	assert (m_pCtl && pPath);
	char pBuf[256];

	if ((strlen (pPath) + 6) > sizeof (pBuf))
		return 0;

	sprintf (pBuf, "MKD %s", pPath);

	return (Sendcmd (pBuf, '2'));
}

/*执行"cd"命令
  入参：pPath - 要"cd"到的目录名（相对或绝对路径都可）
  出参：成功返回1，失败返回0
*/
int yCFtp::Cd (const char *pPath)
{
	assert (m_pCtl && pPath);
	char pBuf[256];

	if ((strlen (pPath) + 6) > sizeof (pBuf))
		return 0;

	sprintf (pBuf, "CWD %s", pPath);

	return (Sendcmd (pBuf, '2'));
}

/*执行"cdup"命令
  入参：无
  出参：成功返回1，失败返回0
*/
int yCFtp::Cdup ()
{
	assert (m_pCtl);

	return (Sendcmd ("CDUP", '2'));
}

/*执行"passive"命令
  入参：无
  出参：成功返回1，失败返回0
*/
int yCFtp::Passive ()
{
	assert (m_pCtl);

	return (Sendcmd ("PASV", '2'));
}

/*执行"rmdir"命令
  入参：pPath - 要删除的目录名（相对或绝对路径都可）
  出参：成功返回1，失败返回0
*/
int yCFtp::Rmdir (const char *pPath)
{
	assert (m_pCtl && pPath);
	char pBuf[256];

	if ((strlen (pPath) + 6) > sizeof (pBuf))
		return 0;

	sprintf (pBuf, "RMD %s", pPath);

	return (Sendcmd (pBuf, '2'));
}

/*执行"pwd"命令
  入参：pPath - "pwd"执行后得到的服务器的当前路径（绝对路径）
  出参：成功返回1，失败返回0
*/
int yCFtp::Pwd(char *pPath)
{
	assert (m_pCtl && pPath);
	char *b = pPath, *s = NULL;

	if (!Sendcmd ("PWD", '2'))
		return 0;

	if ((s = strchr (m_pCtl->pResponse, '"')) == NULL)
		return 0;

	s++;

	while ((*s) && (*s != '"'))
		*b++ = *s++;

	*b++ = '\0';

	return 1;
}

/*执行"nlist"命令(即把指定路径下的所有文件（不包括目录）的信息返回来)
  入参：pOutputfile - 信息返回后保存到这个文件里，如果pOutputfile为NULL，则信息直接打印到屏幕
  	pPath - 指定的路径
  	nMode - 以文本或二进制模式返回，缺省是二进制模式（文本：'A' ；二进制：'I'）
  出参：成功返回1，失败返回0
*/
int yCFtp::Nlist (const char *pOutputfile, const char *pPath, int nMode)
{
	return Xfer (pOutputfile, pPath, FTPLIB_DIR, nMode);
}

/*执行"ls"命令(即把指定路径下的所有文件、目录的信息返回来)
  入参：pOutputfile - 信息返回后保存到这个文件里，如果pOutputfile为NULL，则信息直接打印到屏幕
  	pPath - 指定的路径
  	nMode - 以文本或二进制模式返回，缺省是二进制模式
  出参：成功返回1，失败返回0
*/
int yCFtp::Ls (const char *pOutputfile, const char *pPath, int nMode)
{
	return Xfer (pOutputfile, pPath, FTPLIB_DIR_VERBOSE, nMode);
}

/*执行"size"命令(即返回指定文件的大小,单位：字节)
  入参：pPath - 指定的文件
  	nSize - 返回文件的大小放入nSize中
  	chMode - 以文本或二进制模式返回，缺省是二进制模式
  出参：成功返回1，失败返回0
*/
int yCFtp::Size (const char *pPath, int *nSize, char chMode)
{
	assert (m_pCtl && pPath && nSize);
	char pCmd[256];
	int resp = 0, sz = 0, rv = 1;

	if ((strlen (pPath) + 7) > sizeof (pCmd))
		return 0;

	sprintf (pCmd, "TYPE %c", chMode);
	if (!Sendcmd (pCmd, '2'))
		return 0;

	sprintf (pCmd, "SIZE %s", pPath);
	if (!Sendcmd (pCmd, '2'))
	{
		rv = 0;
	}
	else
	{
		if (sscanf (m_pCtl->pResponse, "%d %d", &resp, &sz) == 2)
			*nSize = sz;
		else
			rv = 0;
	}

	return rv;
}

/*执行"get"命令（从服务器下载某个文件）
  入参：pOutputfile - 下载的文件保存在这个文件中
  	pPath - 要下载的文件
  	nMode - 以文本或二进制模式下载，缺省是二进制模式
  出参：成功返回1，失败返回0
*/
int yCFtp::Get (const char *pOutputfile, const char *pPath, int nMode)
{
	return Xfer (pOutputfile, pPath, FTPLIB_FILE_READ, nMode);
}

/*执行"put"命令（往服务器上传某个文件）
  入参：pOutputfile - 要上传的文件
  	pPath - 上传的文件保存到服务器的这个文件中
  	nMode - 以文本或二进制模式上传，缺省是二进制模式
  出参：成功返回1，失败返回0
*/
int yCFtp::Put (const char *pInputfile, const char *pPath, int nMode)
{
	return Xfer (pInputfile, pPath, FTPLIB_FILE_WRITE, nMode);
}

/*执行"rename"命令
  入参：pSrc - 源文件名
  	pDst - 目的文件名
  出参：成功返回1，失败返回0
*/
int yCFtp::Rename (const char *pSrc, const char *pDst)
{
	assert (m_pCtl && pSrc && pDst);
	char pCmd[256];

	if (((strlen (pSrc) + 7) > sizeof (pCmd)) || ((strlen (pDst) + 7) > sizeof (pCmd)))
		return 0;
	
	sprintf (pCmd, "RNFR %s", pSrc);

	if (!Sendcmd (pCmd, '3'))
		return 0;

	sprintf (pCmd, "RNTO %s", pDst);

	return (Sendcmd (pCmd, '2'));
}

/*执行"delete"命令
  入参：pFilename - 要删除的文件名
  出参：成功返回1，失败返回0
*/
int yCFtp::Delete (const char *pFilename)
{
	assert (m_pCtl && pFilename);
	char pCmd[256];

	if ((strlen (pFilename) + 7) > sizeof (pCmd))
		return 0;

	sprintf (pCmd, "DELE %s", pFilename);

	return (Sendcmd (pCmd, '2'));
}

/*执行"quit"命令
  入参：无
  出参：成功返回1，失败返回0
*/
int yCFtp::Quit ()
{
	assert (m_pCtl);
	Sendcmd ("QUIT", '2');
	Empty ();

	return 1;
}

/*清空m_pCtl内存
  入参：无
  出参：成功返回1，失败返回0
*/
int yCFtp::Empty ()
{
	if (m_pCtl)
	{
		if (m_pCtl->nHandle != -1)
		{
			close (m_pCtl->nHandle);
			m_pCtl->nHandle = -1;
		}

		if (m_pCtl->pBuf)
  		{
			free (m_pCtl->pBuf);
			m_pCtl->pBuf = NULL;
		}

  		free (m_pCtl);
		m_pCtl = NULL;
	}

	return 1;
}

/*等待socket有数据
  入参：pCtl - netbuf结构
  出参：成功返回1，失败返回0
*/
int yCFtp::Socketwait (netbuf *pCtl)
{
	assert (m_pCtl);
	fd_set fd, *rfd = NULL, *wfd = NULL;
	struct timeval tv;
	int rv = 0;

	if ((pCtl->nDir == FTPLIB_CONTROL) || (pCtl->funIdlecb == NULL))
		return 1;

	if (pCtl->nDir == FTPLIB_WRITE)
		wfd = &fd;
	else
		rfd = &fd;

	FD_ZERO (&fd);

	do
	{
		FD_SET (pCtl->nHandle, &fd);
		tv = pCtl->stIdletime;
		rv = select (pCtl->nHandle + 1, rfd, wfd, NULL, &tv);

		if (rv == -1)
		{
			rv = 0;
			strncpy (pCtl->pCtrl->pResponse, strerror (errno),
				sizeof (pCtl->pCtrl->pResponse));
			break;
		}
		else if (rv > 0)
		{
			rv = 1;
			break;
		}
	}
	while ((rv = pCtl->funIdlecb (pCtl, pCtl->nFered, pCtl->pIdleary)));

	return rv;
}

/*读一行
  入参：pBuf - 读出的内容放入pBuf中
  	nMax - 一行的最大字节数
  	pCtl - netbuf结构
  出参：成功返回读的字节数，失败返回-1
*/
int yCFtp::Readline (char *pBuf, int nMax, netbuf *pCtl)
{
	assert (m_pCtl && pBuf && (nMax > 0) && pCtl);
	int x = 0, nRetval = 0, nEof = 0;
	char *pEnd = NULL, *pBeg = pBuf;

	if ((pCtl->nDir != FTPLIB_CONTROL) && (pCtl->nDir != FTPLIB_READ))
		return -1;

	do
	{
		if (pCtl->nAvail > 0)
		{
			x = (nMax >= pCtl->nAvail) ? pCtl->nAvail:(nMax - 1);

			if ((pEnd = (char *) memccpy (pBeg, pCtl->pGet, '\n', x)))
				x = pEnd-pBeg;

			nRetval += x;
			pBeg += x;
			*pBeg = '\0';
			nMax -= x;
			pCtl->pGet += x;
			pCtl->nAvail -= x;

			if (pEnd)
			{
				pBeg -= 2;
				if (strcmp (pBeg, "\r\n") == 0)
				{
					*pBeg++ = '\n';
					*pBeg++ = '\0';
					--nRetval;
				}
				break;
			}
		}

		if (nMax == 1)
		{
			*pBuf = '\0';
			break;
		}

		if (pCtl->pPut == pCtl->pGet)
		{
			pCtl->pPut = pCtl->pGet = pCtl->pBuf;
			pCtl->nAvail = 0;
			pCtl->nLeft = FTPLIB_BUFSIZ;
		}

		if (nEof)
		{
			if (!nRetval)
				nRetval = -1;
			break;
		}

		if (!Socketwait (pCtl))
			return nRetval;

		if ((x = read (pCtl->nHandle, pCtl->pPut, pCtl->nLeft)) == -1)
		{
			nRetval = -1;
			break;
		}

		if (!x) nEof = 1;

		pCtl->nLeft -= x;
		pCtl->nAvail += x;
		pCtl->pPut += x;
	}
	while (1);

	return nRetval;
}

/*写一行
  入参：pBuf - 存放行的内容
  	nLen - 行的字节数
  	pData - netbuf结构
  出参：成功返回写的字节数，失败返回-1
*/
int yCFtp::Writeline (const char *pBuf, int nLen, netbuf *pData)
{
	assert (m_pCtl && pBuf && (nLen >= 0) && pData);
	int x = 0, nb = 0, w = 0;
	const char *ubp = pBuf;
	char *nbp = NULL, lc = 0;

	if (pData->nDir != FTPLIB_WRITE)
    		return -1;

	nbp = pData->pBuf;
	for (x = 0; x < nLen; x++)
	{
		if ((*ubp == '\n') && (lc != '\r'))
		{
			if (nb == FTPLIB_BUFSIZ)
			{
				if (!Socketwait (pData))
					return x;

				w = write (pData->nHandle, nbp, FTPLIB_BUFSIZ);

				if (w != FTPLIB_BUFSIZ)
				{
					printf ("write(1) returned %d, errno = %d\n", w, errno);
					return -1;
				}
				nb = 0;
			}
			nbp[nb++] = '\r';
		}

		if (nb == FTPLIB_BUFSIZ)
		{
			if (!Socketwait (pData))
				return x;
			w = write (pData->nHandle, nbp, FTPLIB_BUFSIZ);
			if (w != FTPLIB_BUFSIZ)
			{
				printf ("write(2) returned %d, errno = %d\n", w, errno);
				return -1;
			}
			nb = 0;
		}
		nbp[nb++] = lc = *ubp++;
	}

	if (nb)
	{
		if (!Socketwait (pData))
			return x;

		w = write (pData->nHandle, nbp, nb);

		if (w != nb)
		{
			printf ("write(3) returned %d, errno = %d\n", w, errno);
			return -1;
		}
	}

	return nLen;
}

/*发送命令
  入参：pCmd - 发送的命令
  	chExpresp - 期望服务器的回应值
  出参：成功返回1，失败返回0
*/
int yCFtp::Sendcmd (const char *pCmd, char chExpresp)
{
	assert (m_pCtl && pCmd);
	char pBuf[256];

	if (m_pCtl->nDir != FTPLIB_CONTROL)
		return 0;

	if ((strlen (pCmd) + 3) > sizeof (pBuf))
		return 0;

	sprintf (pBuf, "%s\r\n", pCmd);

	if (write (m_pCtl->nHandle, pBuf, strlen (pBuf)) <= 0)
		return 0;

	return Readresp (chExpresp, m_pCtl);
}

/*发送命令后接收服务器的回应
  入参：c - 期望服务器的回应值
  	pCtl - netbuf结构
  出参：成功返回1，失败返回0
*/
int yCFtp::Readresp (char c, netbuf *pCtl)
{
	assert (m_pCtl && pCtl);
	char pMatch[5];

	if (Readline (pCtl->pResponse, 256, pCtl) == -1)
		return 0;

	if (pCtl->pResponse[3] == '-')
	{
		strncpy (pMatch, pCtl->pResponse, 3);
		pMatch[3] = ' ';
		pMatch[4] = '\0';
		do
		{
			if (Readline (pCtl->pResponse, 256, pCtl) == -1)
				return 0;
		}
		while (strncmp (pCtl->pResponse, pMatch, 4));
	}

	if (pCtl->pResponse[0] == c)
		return 1;

	return 0;
}

/*传输数据（接收或发送）
  入参：pLocalfile - 本地文件。如果是接收，则接收到的文件存入此文件中；如果是发送，则发送此文件
  	pPath - 服务器端文件。如果是接收，则接收此文件；如果是发送，则发送的文件存入此文件中
  	nType - 传输类型
  	nMode - 传输模式，缺省是二进制模式
  出参：成功返回1，失败返回0
*/
int yCFtp::Xfer (const char *pLocalfile, const char *pPath, int nType, int nMode)
{
	assert (m_pCtl);
	int l = 0, c = 0;
	char *pBuf = NULL;
	FILE *fpLocal = NULL;
	netbuf *pData = NULL;

	char *backuplog = "/tmp/backup.log";
	FILE *logfp = fopen(backuplog, "a");

/*
	if (NULL != logfp)
	{
		fprintf(logfp, "%s:", pLocalfile);
	}
*/

	if (pLocalfile)
	{
		if ((fpLocal = fopen (pLocalfile, (nType == FTPLIB_FILE_WRITE) ? "r" : "w")) == NULL)
		{
			strncpy (m_pCtl->pResponse, strerror (errno), sizeof (m_pCtl->pResponse));

			if (NULL != logfp)
			{
				fprintf(logfp, "%s: fopen error!\n", pLocalfile);
			}
			fclose(logfp);
			return 0;
		}
	}

	if (!fpLocal)
		fpLocal = (nType == FTPLIB_FILE_WRITE) ? stdin : stdout;

	if (!DataConnect (pPath, nType, nMode, &pData, logfp, pLocalfile))
	{
		fclose(logfp);
		fclose(fpLocal);
		return 0;
	}

	pBuf = (char *) malloc (FTPLIB_BUFSIZ);

	if (nType == FTPLIB_FILE_WRITE)
	{
		while ((l = fread (pBuf, 1, FTPLIB_BUFSIZ, fpLocal)) > 0)
			if ((c = Write (pBuf, l, pData)) < l)
				printf ("short write: passed %d, wrote %d\n", l, c);
	}
	else
	{
		while ((l = Read (pBuf, FTPLIB_BUFSIZ, pData)) > 0)
			if (fwrite (pBuf, 1, l, fpLocal) <= 0)
				break;
	}

	free (pBuf);
	fflush (fpLocal);

	if (pLocalfile) fclose (fpLocal);

	int ret = CloseData (pData);
	if (0 == ret)
	{
		if (NULL != logfp)
		{
			fprintf(logfp, "%s: CloseData error!\n", pLocalfile);
		}
	}	
	/*
	else
	{
		if (NULL != logfp)
		{
			fprintf(logfp, "%s: put successfully!\n", pLocalfile);
		}
	}
	*/

	if (NULL != logfp)
	{
		fclose(logfp);
		logfp = NULL;
	}

	return ret;
}

/*打开数据端口
  入参：pData - 打开后返回的netbuf结构
  	nMode - connection modes
  	nDir - 类型
  出参：成功返回1，失败返回0
*/
int yCFtp::OpenPort (netbuf **pData, int nMode, int nDir)
{
	assert (m_pCtl);
	int nData;
	union
	{
		struct sockaddr sa;
		struct sockaddr_in in;
	}sin;
	struct linger lng = {0, 0};
	unsigned int l, v[6];
	int on = 1;
	netbuf *pCtrl = NULL;
	char *cp = NULL, pBuf[256];

	if (m_pCtl->nDir != FTPLIB_CONTROL)
		return 0;

	if ((nDir != FTPLIB_READ) && (nDir != FTPLIB_WRITE))
	{
		sprintf (m_pCtl->pResponse, "Invalid direction %d\n", nDir);
		return 0;
	}

	if ((nMode != FTPLIB_ASCII) && (nMode != FTPLIB_IMAGE))
	{
		sprintf (m_pCtl->pResponse, "Invalid mode %c\n", nMode);
		return 0;
	}

	l = sizeof (sin);

	if (m_pCtl->nMode == FTPLIB_PASSIVE)
	{
		memset (&sin, 0, l);
		sin.in.sin_family = AF_INET;

		if (!Sendcmd ("PASV", '2'))
			return 0;

		if ((cp = strchr (m_pCtl->pResponse, '(')) == NULL)
			return 0;

		cp++;
		sscanf (cp, "%u,%u,%u,%u,%u,%u", &v[2], &v[3], &v[4], &v[5], &v[0], &v[1]);
		sin.sa.sa_data[2] = v[2];
		sin.sa.sa_data[3] = v[3];
		sin.sa.sa_data[4] = v[4];
		sin.sa.sa_data[5] = v[5];
		sin.sa.sa_data[0] = v[0];
		sin.sa.sa_data[1] = v[1];
	}
	else
	{
		if (getsockname (m_pCtl->nHandle, &sin.sa, &l) < 0)
			return 0;
	}

	if ((nData = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		return 0;

	if (setsockopt (nData, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof (on)) == -1)
	{
		close (nData);
		return 0;
	}

	if (setsockopt (nData, SOL_SOCKET, SO_LINGER, (void *) &lng, sizeof (lng)) == -1)
	{
		close (nData);
		return 0;
	}

	if (m_pCtl->nMode == FTPLIB_PASSIVE)
	{
		if (connect (nData, &sin.sa, sizeof (sin.sa)) == -1)
		{
			close (nData);
			return 0;
		}
	}
	else
	{
		sin.in.sin_port = 0;

		if (bind(nData, &sin.sa, sizeof (sin)) == -1)
		{
			close (nData);
			return 0;
		}

		if (listen (nData, 1) < 0)
		{
			close (nData);
			return 0;
		}

		if (getsockname (nData, &sin.sa, &l) < 0)
			return 0;

		sprintf (pBuf, "PORT %d,%d,%d,%d,%d,%d",
			(unsigned char) sin.sa.sa_data[2],
			(unsigned char) sin.sa.sa_data[3],
			(unsigned char) sin.sa.sa_data[4],
			(unsigned char) sin.sa.sa_data[5],
			(unsigned char) sin.sa.sa_data[0],
			(unsigned char) sin.sa.sa_data[1]);

		if (!Sendcmd (pBuf, '2'))
		{
			close (nData);
			return 0;
		}
	}

	pCtrl = (netbuf *) malloc (sizeof (netbuf));

	if (nMode == 'A')
		pCtrl->pBuf = (char *) malloc (FTPLIB_BUFSIZ);
	else
		pCtrl->pBuf = NULL;

	pCtrl->nHandle = nData;
	pCtrl->nDir = nDir;
	pCtrl->pCtrl = (m_pCtl->nMode == FTPLIB_PASSIVE) ? m_pCtl : NULL;
	pCtrl->stIdletime = m_pCtl->stIdletime;
	pCtrl->pIdleary = m_pCtl->pIdleary;
	pCtrl->nFered = 0;
	pCtrl->nFeredl = 0;
	pCtrl->nBytes = m_pCtl->nBytes;

	if (pCtrl->stIdletime.tv_sec | pCtrl->stIdletime.tv_usec)
		pCtrl->funIdlecb = m_pCtl->funIdlecb;
	else
		pCtrl->funIdlecb = NULL;

	*pData = pCtrl;

	return 1;
}

/*接受连接
  入参：pData - netbuf结构
  出参：成功返回1，失败返回0
*/
int yCFtp::AcceptConnection (netbuf *pData)
{
	assert (m_pCtl && pData);
	int sData = -1;
	struct sockaddr addr;
	unsigned int l = 0;
	int i = 0, rv = 0;
	struct timeval tv;
	fd_set mask;

	FD_ZERO (&mask);
	FD_SET (m_pCtl->nHandle, &mask);
	FD_SET (pData->nHandle, &mask);
	tv.tv_usec = 0;
	tv.tv_sec = ACCEPT_TIMEOUT;

	i = m_pCtl->nHandle;
	if (i < pData->nHandle)
		i = pData->nHandle;

	i = select ( i + 1, &mask, NULL, NULL, &tv);

	if(i == -1)
	{
		strncpy (m_pCtl->pResponse, strerror (errno), sizeof (m_pCtl->pResponse));
		close (pData->nHandle);
		pData->nHandle = 0;
		rv = 0;
	}
	else if (i == 0)
	{
		strcpy (m_pCtl->pResponse, "timed out waiting for connection");
		close (pData->nHandle);
		pData->nHandle = 0;
		rv = 0;
	}
	else
	{
		if (FD_ISSET (pData->nHandle, &mask))
		{
			l = sizeof (addr);
			sData = accept (pData->nHandle, &addr, &l);
			i = errno;
			close (pData->nHandle);
			if (sData > 0)
			{
				rv = 1;
				pData->nHandle = sData;
				pData->pCtrl = m_pCtl;
			}
			else
			{
				strncpy (m_pCtl->pResponse, strerror (i), sizeof (m_pCtl->pResponse));
				pData->nHandle = 0;
				rv = 0;
			}
		}
		else if (FD_ISSET (m_pCtl->nHandle, &mask))
		{
			close (pData->nHandle);
			pData->nHandle = 0;
			Readresp ('2', m_pCtl);
			rv = 0;
		}
	}

	return rv;
}

/*数据连接
  入参：pPath - 服务器端文件
  	nType - connection modes
  	nMode - mode codes
  	pData - 数据连接后返回的netbuf结构
  出参：成功返回1，失败返回0
*/
int yCFtp::DataConnect (const char *pPath, int nType, int nMode, netbuf **pData, FILE *logfp, const char *filename)
{
	assert (m_pCtl);
	char pBuf[256];
	int nDir = 0;

	if ((pPath == NULL) && ((nType == FTPLIB_FILE_WRITE) || (nType == FTPLIB_FILE_READ)))
	{
		sprintf (m_pCtl->pResponse, "Missing path argument for file transfer\n");
		return 0;
	}

	sprintf (pBuf, "TYPE %c", nMode);

	if (!Sendcmd (pBuf, '2'))
	{
		if (NULL != logfp)
		{
			fprintf(logfp, "%s: Sendcmd (pBuf, 2) error!\n", filename);
		}
	
		return 0;
	}

	switch (nType)
	{
	case 1://FTPLIB_DIR
		strcpy (pBuf, "NLST");
		nDir = FTPLIB_READ;
		break;

	case 2://FTPLIB_DIR_VERBOSE:
		strcpy (pBuf, "LIST");
		nDir = FTPLIB_READ;
		break;

	case 3://FTPLIB_FILE_READ:
		strcpy (pBuf, "RETR");
		nDir = FTPLIB_READ;
		break;

	case 4://FTPLIB_FILE_WRITE:
		strcpy (pBuf, "STOR");
		nDir = FTPLIB_WRITE;
		break;

	default:
		sprintf (m_pCtl->pResponse, "Invalid open type %d\n", nType);
		return 0;
	}

	if (pPath)
	{
		int i = strlen (pBuf);
		pBuf[i++] = ' ';
		if ((strlen (pPath) + i) >= sizeof (pBuf))
			return 0;
		strcpy (&pBuf[i], pPath);
	}

	if (!OpenPort (pData, nMode, nDir))
	{
		if (NULL != logfp)
		{
			fprintf(logfp, "%s: OpenPort error!\n", filename);
		}
		return 0;
	}

	if (!Sendcmd (pBuf, '1'))
	{
		(*pData)->pCtrl = NULL;
		CloseData (*pData);
		*pData = NULL;

		if (NULL != logfp)
		{
			fprintf(logfp, "%s: Sendcmd (pBuf, 1) error!\n", filename);
		}
		
		return 0;
	}

	if (m_pCtl->nMode == FTPLIB_PORT)
	{
		if (!AcceptConnection (*pData))
		{
			CloseData (*pData);
			*pData = NULL;

			if (NULL != logfp)
			{
				fprintf(logfp, "%s: AcceptConnection error!\n", filename);
			}
			
			return 0;
		}
	}

/*
	if (NULL != logfp)
	{
		fprintf(logfp, "%s: DataConnect Successfully!\n", filename);
	}
*/

	return 1;
}

/*关闭数据连接
  入参：pData - netbuf结构
  出参：成功返回1，失败返回0
*/
int yCFtp::CloseData (netbuf *pData)
{
	assert (m_pCtl && pData);
	netbuf *pCtrl = NULL;

	if ((pData->nDir != FTPLIB_READ) && (pData->nDir != FTPLIB_WRITE))
		return 0;

	if (pData->pBuf)
		free (pData->pBuf);

	shutdown (pData->nHandle, 2);
	close (pData->nHandle);
	pCtrl = pData->pCtrl;
	free (pData);

	if (pCtrl)
		return (Readresp ('2', pCtrl));

	return 1;
}

/*读数据
  入参：pBuf - 读出的数据放入pBuf中
  	nMax - 允许读的最大字节数
  	pData - netbuf结构
  出参：成功返回读出的字节数，失败返回-1
*/
int yCFtp::Read (char *pBuf, int nMax, netbuf *pData)
{
	assert (m_pCtl && pData);
	int i = 0;

	if (pData->nDir != FTPLIB_READ)
		return 0;

	if(pData->pBuf)
	{
		i = Readline (pBuf, nMax, pData);
	}
	else
	{
		Socketwait (pData);
		i = read (pData->nHandle, pBuf, nMax);
	}

	pData->nFered += i;
	if (pData->funIdlecb && pData->nBytes)
	{
		pData->nFeredl += i;
		if (pData->nFeredl > pData->nBytes)
		{
			pData->funIdlecb (pData, pData->nFered, pData->pIdleary);
			pData->nFeredl = 0;
		}
	}

	return i;
}

/*写数据
  入参：pBuf - 存放要写的数据
  	nLen - 数据长度
  	pData - netbuf结构
  出参：成功返回已写的字节数，失败返回-1
*/
int yCFtp::Write (const char *pBuf, int nLen, netbuf *pData)
{
	assert (m_pCtl && pData);
	int i = 0;

	if (pData->nDir != FTPLIB_WRITE)
		return 0;

	if (pData->pBuf)
	{
		i = Writeline (pBuf, nLen, pData);
	}
	else
	{
		Socketwait (pData);
		i = write (pData->nHandle, pBuf, nLen);
	}

	pData->nFered += i;
	if (pData->funIdlecb && pData->nBytes)
	{
		pData->nFeredl += i;
		if (pData->nFeredl > pData->nBytes)
		{
			pData->funIdlecb (pData, pData->nFered, pData->pIdleary);
			pData->nFeredl = 0;
		}
	}

	return i;
}
