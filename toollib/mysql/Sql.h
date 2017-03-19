/*****************************************************************************\
Copyright (c) 2010 Shenzhen Surfilter Network Technology Co., Ltd. All Rights Reserved.

Module Name:
  MySQL的常用操作

Abstract:
  将MySQL的常用操作封装到类中

Notes:
  C接口connect_mysql可以供C代码调用，用来连接mysql

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
	Sql(string config_file = CONFIG_FILE_NAME);//默认的配置文件为CONFIG_FILE_NAME
	~Sql();	
	int Connect();
	void Close();                    //关闭MySQL连接
	int Execute(const char *cmd);   //插入、删除、更新语句
	int Query(const char *cmd);     //查询语句
	int GetRowCount();              //得到行数
	int GetColCount();              //得到列数
	const char* GetColName(const int &index);      //得到列名
	MYSQL_ROW GetRow();             //返回一行
	unsigned int GetInsertID();    //得到自增序列的值
   static char *Escape(char field[], int nMaxLen);
	const char* Error();            //出错信息
	unsigned int ErrorNo();
	void FreeResult();              //释放结果集
	  //连接MySQL，端口号port可不传
	int Connect(const char *host, const char *username, const char *passwd, const char *dbname, unsigned int port = SQLPORT);
private:
  
	MYSQL m_connection;             //MySQL连接
	MYSQL_RES *m_result;            //查询语句的结果集 
	bool IsConnected;
	string m_filename;
};

/*
    connect_mysql是留给C语言调用的接口，编译时需要连接libsql.so
*/
extern "C" int connect_mysql(const char *config_file, MYSQL *db_connection, char *charset, int charset_len);

#endif
