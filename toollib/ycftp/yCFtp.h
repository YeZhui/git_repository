#ifndef __YCFTP_H__
#define __YCFTP_H__

/* 
	Copyright (C) 2000 uchen (uchen@sohu.com)

	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.
*/

#include<sys/time.h>
#include<iostream>
#include<stdio.h>
#include <assert.h>
using namespace std;

typedef struct NetBuf netbuf;
typedef int (*FtpCallback) (netbuf *pCtl, int nFered, void *pArg);

struct NetBuf
{
	char *pPut, *pGet;
	int nHandle;
	int nAvail, nLeft;
	char *pBuf;
	int nDir;
	netbuf *pCtrl;
	int nMode;
	struct timeval stIdletime;
	FtpCallback funIdlecb;
	void *pIdleary;
	int nFered;
	int nBytes;
	int nFeredl;
	char pResponse[256];
};

//Ftp类
class yCFtp
{
//构造、析构函数
public:
	yCFtp();//构造函数
	virtual ~yCFtp();//析构函数

//公有成员函数
public:
	int Connect (const char *, int nsec = 5);//连接Ftp服务器
	int Login (const char *, const char *);//登录Ftp服务器
	int Connectlogin (const char *, const char *, const char *);//连接并登录Ftp服务器
	int Site (const char *);//执行"site"命令
	int Mkdir (const char *);//执行"mkdir"命令
	int Cd (const char *);//执行"cd"命令
	int Cdup ();//执行"cdup"命令
	int Passive(); //执行"PASV"命令
	int Rmdir (const char *);//执行"rmdir"命令
	int Pwd (char *);//执行"pwd"命令
	int Nlist (const char *, const char *, int ='I');//执行"nlist"命令
	int Ls (const char *, const char *, int ='I');//执行"ls"命令
	int Size (const char *, int *, char ='I');//执行"size"命令
	int Get (const char *, const char *, int ='I');//执行"get"命令
	int Put (const char *, const char *, int ='I');//执行"put"命令
	int Getdir (const char *, const char *, int ='I');//执行"get"命令
	int Putdir (const char *, const char *, int ='I');//执行"put"命令
	int Rename (const char *, const char *);//执行"rename"命令
	int Delete (const char *);//执行"delete"命令
	int Quit ();//执行"quit"命令

//私有成员函数
private:
	int Empty();//清空m_pCtl内存
	int Socketwait(netbuf *);//等待socket有数据
	int Readline(char *, int,netbuf *);//读一行
	int Writeline(const char *, int,netbuf *);//写一行
	int Sendcmd(const char *, char);//发送命令
	int Readresp(char, netbuf *);//发送命令后接收服务器的回应
	int Xfer(const char *, const char *, int,int);//传输数据（接收或发送）
	int OpenPort(netbuf **, int, int);//打开数据端口
	int AcceptConnection(netbuf *);//接受连接
	int DataConnect(const char *, int, int, netbuf **, FILE *, const char *);//数据连接
	int CloseData(netbuf *);//关闭数据连接
	int Read(char *, int, netbuf *);//读数据
	int Write(const char *, int,netbuf *);//写数据

//私有变量
private:
	/* type codes */
	int FTPLIB_DIR;
	int FTPLIB_DIR_VERBOSE;
	int FTPLIB_FILE_READ;
	int FTPLIB_FILE_WRITE;

	/* mode codes */
	char FTPLIB_ASCII;
	char FTPLIB_IMAGE;
	char FTPLIB_TEXT;
	char FTPLIB_BINARY;

	/* connection modes */
	int FTPLIB_PASSIVE;
	int FTPLIB_PORT;

	/* connection option names */
	int FTPLIB_CONNMODE;
	int FTPLIB_CALLBACK;
	int FTPLIB_IDLETIME;
	int FTPLIB_CALLBACKARG;
	int FTPLIB_CALLBACKBYTES;

	int FTPLIB_BUFSIZ;
	int ACCEPT_TIMEOUT;
	int FTPLIB_CONTROL;
	int FTPLIB_READ;
	int FTPLIB_WRITE;
	int FTPLIB_DEFMODE;

//私有变量
private:
	netbuf *m_pCtl;
};

#endif
