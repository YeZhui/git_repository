/*****************************************************************************\
Copyright (c) 2010 Shenzhen Surfilter Network Technology Co., Ltd. All Rights Reserved.

Module Name:
  MySQL�ĳ��ò���

Abstract:
  ��MySQL�ĳ��ò�����װ������

Notes:
  C�ӿ�connect_mysql���Թ�C������ã���������mysql

Created: 2010-1-22
\*****************************************************************************/
#ifndef __MYSQL_OPERATION_LIB
#define __MYSQL_OPERATION_LIB

#include <string.h>
#include <stdio.h>
#include <mysql/mysql.h>

#include <string>
using std::string;

#include "../ReadIni/ReadIni.h"

//#define CONFIG_FILE_NAME "/apps/www/htdocs/application/configs/application.ini"
#define CONFIG_FILE_NAME "/home/rtx/run_env/plcy.d/application.ini"
#define SQLPORT 0

class Sql
{
public:
	Sql(string config_file = CONFIG_FILE_NAME);//Ĭ�ϵ������ļ�ΪCONFIG_FILE_NAME
	~Sql();	
	int Connect();
	void Close();                    //�ر�MySQL����
	int Execute(const char *cmd);   //���롢ɾ�����������
	int Query(const char *cmd);     //��ѯ���
	int GetRowCount();              //�õ�����
	int GetColCount();              //�õ�����
	const char* GetColName(const int &index);      //�õ�����
	MYSQL_ROW GetRow();             //����һ��
	unsigned int GetInsertID();    //�õ��������е�ֵ
   static char *Escape(char field[], int nMaxLen);
	const char* Error();            //������Ϣ
	unsigned int ErrorNo();
	void FreeResult();              //�ͷŽ����
	  //����MySQL���˿ں�port�ɲ���
	int Connect(const char *host, const char *username, const char *passwd, const char *dbname, unsigned int port = SQLPORT);
private:
  
	MYSQL m_connection;             //MySQL����
	MYSQL_RES *m_result;            //��ѯ���Ľ���� 
	bool IsConnected;
	string m_filename;
};

/*
    connect_mysql������C���Ե��õĽӿڣ�����ʱ��Ҫ����libsql.so
*/
extern "C" int connect_mysql(const char *config_file, MYSQL *db_connection, char *charset, int charset_len);

#endif
