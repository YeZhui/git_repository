#include "Sql.h"

/*
    函数功能: 从配置文件中读取MySQL的配置信息
	参数：
	    file_name  配置文件名
		host       MySQL服务器地址
		username   用户名
		password   密码
		dbname     数据库名
		charset    字符集
	返回值：
	    -1 读取配置文件失败
		0  读取配置文件成功
*/
int read_config(const string &file_name, string &host, string &username, string &password,
                string &dbname, string &charset)
{
    ReadIni file(file_name, string(";"));//以;开头的是注释
	if (-1 == file.Init())//初始化失败
	    return -1;
	
	//获取配置参数
	host = file.Query(string("production"), string("resources.db.params.host"));
	username = file.Query(string("production"), string("resources.db.params.username"));	
	password = file.Query(string("production"), string("resources.db.params.password"));	
	dbname = file.Query(string("production"), string("resources.db.params.dbname"));
	charset = file.Query(string("production"), string("resources.db.params.charset"));
	
	return 0;
}


/*
    函数功能: 构造函数
	参数：
	    config_file  配置文件的路径
	返回值：无
*/
Sql::Sql(string config_file)
{
    //配置文件名
	m_filename = config_file;
	
    //初始值为没有连接	
	IsConnected = false;
	//将结果集初始化为NULL
	m_result = NULL;
}

/*
    函数功能：析构函数，关闭MySQL连接并释放结果集
	参数：无
	返回值：无
*/
Sql::~Sql()
{
    if (IsConnected)
	    Close();
	
	if (m_result)
	    FreeResult();
}

/*
    函数功能：连接MySQL服务器
	参数：无
	返回值：
	    -1   连接失败
		0    连接成功
*/
int Sql::Connect()
{	
	//如果已经连接则直接返回
	if (IsConnected)
	    return 0;
	
	string host, username, password, dbname, charset;
	//读取配置文件失败就采用默认的配置
	if (-1 == read_config(m_filename, host, username, password, dbname, charset))
	{
	    	host = "127.0.0.1";
		username = "root";
		password = "";
		dbname = "rzxdb";
		charset = "utf8";
	}
	
	//连接服务器
	if (0 == Connect(host.c_str(), username.c_str(), password.c_str(), dbname.c_str()))
	{
	    IsConnected = true;
				
		//连接服务器成功之后设置字符集
		char cmd[32] = {0};
		snprintf(cmd, sizeof(cmd) - 1, "set names %s", charset.c_str());	
		Execute(cmd);//执行SQL语句，设置字符集
		
		return 0;
	}
	else
	{
	    IsConnected = false;	
		return -1;
	}
}

/*
    函数功能：连接MySQL
	参数：
	    host      服务器地址
		username  用户名
		passwd    密码
		dbname    数据库名
		port      端口号
	返回值：
	    -1     连接失败
		0      连接成功
*/
int Sql::Connect(const char *host, const char *username, const char *passwd, const char *dbname, unsigned int port)
{
    if (NULL == host || NULL == username || NULL == passwd || NULL == dbname || port < 0)
	    return -1;

	//连接MySQL服务器
	mysql_init(&m_connection);//初始化MYSQL连接
	int n;
    	n = 20; // 设定20秒超时
    	mysql_options(&m_connection, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&n);
    	n = 1;  // 断后自动重连
    	mysql_options(&m_connection, MYSQL_OPT_RECONNECT, (char *)&n);
	
	if (NULL != mysql_real_connect(&m_connection, host, username, passwd, dbname, port, NULL, CLIENT_FOUND_ROWS))
	    return 0;
	else
	    return -1;
}

/*
    函数功能：关闭MySQL连接
	参数：无
	返回值：无
*/
void Sql::Close()
{
    if (IsConnected)
	{
	    mysql_close(&m_connection);
		IsConnected = false;
	}
}

/*
    函数功能：执行插入、更新、删除等功能
	参数：
	    cmd      insert、delete、update等不需要返回结果集的SQL语句
	返回值：
	    0    成功
		非0  失败
*/
int Sql::Execute(const char *cmd)
{
    if (NULL == cmd || !IsConnected)
	    return -1;
		
	//insert、delete、update等操作不必释放结果集	
	return mysql_query(&m_connection, cmd);
}

/*
    函数功能：执行查询语句，获得结果集
	参数：
	    cmd      select语句
	返回值：
	    0    成功
		-1   失败
*/
int Sql::Query(const char *cmd)
{
    if (NULL == cmd || !IsConnected)
	    return -1;
	
	FreeResult();//释放结果集
	
	if (mysql_query(&m_connection, cmd) < 0)//查询
		return -1;
	
	m_result = mysql_store_result(&m_connection);//保存结果集
	
	if (NULL == m_result)
	    return -1;
		
	return 0;
}

/*
    函数功能：返回结果集中的行数
	参数：无
	返回值：
	    -1     结果集为空，无法得到行数
		其他   结果集中的行数
*/
int Sql::GetRowCount()
{
    //结果集必须不为NULL
	if (NULL != m_result)
	    return mysql_num_rows(m_result);
	else
	    return -1;
}

/*
    函数功能：返回结果集中的列数
	参数：无
	返回值：
	    -1     结果集为空，无法得到列数
		其他   结果集中的列数
*/
int Sql::GetColCount()
{
    //结果集必须不为NULL
	if (NULL != m_result)
	    return mysql_num_fields(m_result);
	else
	    return -1;
}

/*
    函数功能：得到结果集中第index列的列名
	参数：
	    index  表示第index列，从0开始
	返回值：
	    NULL   无法得到列名
		其他   第index列的列名
*/
const char* Sql::GetColName(const int &index)
{
    //在结果集不为空的情况下才去获取列名
	if (NULL != m_result)
	{
	    MYSQL_FIELD *field;
	    field = mysql_fetch_field_direct(m_result, index);
		if (NULL == field)//获取失败就直接返回NULL
		    return NULL;
		
		return field->name;
	}
	
	return NULL;
}

/*
    函数功能：得到结果集中的一行
	参数：无
	返回值：
	    NULL   结果集为空，无法得到行
		其他   结果集中的一行
*/
MYSQL_ROW Sql::GetRow()
{
    //结果集必须不为NULL
	if (NULL != m_result)
	    return mysql_fetch_row(m_result);
	
	return NULL;
}

/*
    函数功能：得到当前自增序列的最大值
	参数：无
	返回值：
	    -1    没有连接MySQL
	    其他  当前自增序列的最大值
*/
unsigned int Sql::GetInsertID()
{
    if (IsConnected)
        return (unsigned int)mysql_insert_id(&m_connection);
	else
	    return -1;
}

/*
 *  功能：用于sql操作时处理字段中的转义字符“'、"、\”
 *  参数：field[in][out]--需要处理的字段串。注意，在调用后，该串将被修改，当存
 *                        在需要转义字符时，请确保它是一个可写的内存（串数组）；
 *        nMaxLen[in]--指定field的最大长度；
 *  编写：Rocky 2008-10-27 11:38:52
 *  返回：field的地址（当超长时，返回原field值，否则返回的是已转义的）；
 */
char *Sql::Escape(char field[], int nMaxLen)
{
    int len, count;
    char *p, *dest;

   // assert(NULL != field);

    // 扫描串中需要转义的字符个数，包括“'、"、\”
    count = 0;
    len = 0;
    for(p=field; '\0' != *p; p++)
    {
        if('\\' == *p || '\'' == *p || '"' == *p)
        {
            count++;
        }
        len++;
    }
    // 没有转义字符，则直接返回原值；
    if(0 == count)
    {
        return field;
    }


    // 从后住前扫描（都是在原串中操作）
    p = field + len;
    dest = field + len + count;

    // 超长，直接返回；
    if(dest - field > nMaxLen)
    {
        return field;
    }

    // 做转换
    for(; p >= field; p--)
    {
        *dest-- = *p;
        // 如是特殊字符，则前面加上转义字符；
        if('\\' == *p || '\'' == *p || '"' == *p)
        {
            *dest-- = '\\';
        }
    }

    return field;
}

/*
    函数功能：返回错误信息
	参数：无
	返回值：
	    如果数据库已经连上则返回错误信息
		否则，返回NULL
*/
const char* Sql::Error()
{
    //如果数据库已经连上，则返回错误信息
	if (IsConnected)
	    return mysql_error(&m_connection);
	
	return NULL;
}

/*
    函数功能：释放结果集
	参数：无
	返回值：无
*/
void Sql::FreeResult()
{
    //如果结果集不为NULL，就释放结果集，并将结果集置为NULL
    if (NULL != m_result)
	{
	    mysql_free_result(m_result);
		m_result = NULL;
	}
}

/*
    函数功能：连接MySQL，这是个C接口
	参数：
	    config_file   配置文件名，为NULL时用默认的配置文件CONFIG_FILE_NAME
		db_connection 数据库连接
		sz_charset    字符集，出参，返回配置文件中的字符集
		charset_len   charset的大小
	返回值：
	    -1 连接失败
		0  连接成功
*/

int connect_mysql(const char *config_file, MYSQL *db_connection, char *sz_charset, int charset_len)
{
    if (NULL == sz_charset || charset_len < 0 || NULL == db_connection)
	    return -1;
	
	//设置文件名
	string file_name;
	if (NULL == config_file)
	    file_name.assign(CONFIG_FILE_NAME);
	else
	    file_name.assign(config_file);
		
	string host, username, password, dbname, charset;
	//读取配置文件出错就采用默认配置
	if (-1 == read_config(file_name, host, username, password, dbname, charset))
	{
	    host = "127.0.0.1";
		username = "root";
		password = "";
		dbname = "rzxdb";
		charset = "utf8";
	}
		
	//连接MySQL数据库
	mysql_init(db_connection);//初始化MYSQL连接
	if (mysql_real_connect(db_connection, host.c_str(), username.c_str(), password.c_str(), 
	                       dbname.c_str(), SQLPORT, NULL, CLIENT_FOUND_ROWS) == NULL)
	    return -1;
	
	//返回字符集
	strncpy(sz_charset, charset.c_str(), charset_len - 1);
	return 0;
}


unsigned int Sql::ErrorNo()
{
		if (IsConnected)
	return mysql_errno(&m_connection); 
	return 0;
}

