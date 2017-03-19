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

//Ftp��
class yCFtp
{
//���졢��������
public:
	yCFtp();//���캯��
	virtual ~yCFtp();//��������

//���г�Ա����
public:
	int Connect (const char *, int nsec = 5);//����Ftp������
	int Login (const char *, const char *);//��¼Ftp������
	int Connectlogin (const char *, const char *, const char *);//���Ӳ���¼Ftp������
	int Site (const char *);//ִ��"site"����
	int Mkdir (const char *);//ִ��"mkdir"����
	int Cd (const char *);//ִ��"cd"����
	int Cdup ();//ִ��"cdup"����
	int Passive(); //ִ��"PASV"����
	int Rmdir (const char *);//ִ��"rmdir"����
	int Pwd (char *);//ִ��"pwd"����
	int Nlist (const char *, const char *, int ='I');//ִ��"nlist"����
	int Ls (const char *, const char *, int ='I');//ִ��"ls"����
	int Size (const char *, int *, char ='I');//ִ��"size"����
	int Get (const char *, const char *, int ='I');//ִ��"get"����
	int Put (const char *, const char *, int ='I');//ִ��"put"����
	int Getdir (const char *, const char *, int ='I');//ִ��"get"����
	int Putdir (const char *, const char *, int ='I');//ִ��"put"����
	int Rename (const char *, const char *);//ִ��"rename"����
	int Delete (const char *);//ִ��"delete"����
	int Quit ();//ִ��"quit"����

//˽�г�Ա����
private:
	int Empty();//���m_pCtl�ڴ�
	int Socketwait(netbuf *);//�ȴ�socket������
	int Readline(char *, int,netbuf *);//��һ��
	int Writeline(const char *, int,netbuf *);//дһ��
	int Sendcmd(const char *, char);//��������
	int Readresp(char, netbuf *);//�����������շ������Ļ�Ӧ
	int Xfer(const char *, const char *, int,int);//�������ݣ����ջ��ͣ�
	int OpenPort(netbuf **, int, int);//�����ݶ˿�
	int AcceptConnection(netbuf *);//��������
	int DataConnect(const char *, int, int, netbuf **, FILE *, const char *);//��������
	int CloseData(netbuf *);//�ر���������
	int Read(char *, int, netbuf *);//������
	int Write(const char *, int,netbuf *);//д����

//˽�б���
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

//˽�б���
private:
	netbuf *m_pCtl;
};

#endif
