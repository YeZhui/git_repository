#include "Sql.h"

/*
    ��������: �������ļ��ж�ȡMySQL��������Ϣ
	������
	    file_name  �����ļ���
		host       MySQL��������ַ
		username   �û���
		password   ����
		dbname     ���ݿ���
		charset    �ַ���
	����ֵ��
	    -1 ��ȡ�����ļ�ʧ��
		0  ��ȡ�����ļ��ɹ�
*/
int read_config(const string &file_name, string &host, string &username, string &password,
                string &dbname, string &charset)
{
    ReadIni file(file_name, string(";"));//��;��ͷ����ע��
	if (-1 == file.Init())//��ʼ��ʧ��
	    return -1;
	
	//��ȡ���ò���
	host = file.Query(string("production"), string("resources.db.params.host"));
	username = file.Query(string("production"), string("resources.db.params.username"));	
	password = file.Query(string("production"), string("resources.db.params.password"));	
	dbname = file.Query(string("production"), string("resources.db.params.dbname"));
	charset = file.Query(string("production"), string("resources.db.params.charset"));
	
	return 0;
}


/*
    ��������: ���캯��
	������
	    config_file  �����ļ���·��
	����ֵ����
*/
Sql::Sql(string config_file)
{
    //�����ļ���
	m_filename = config_file;
	
    //��ʼֵΪû������	
	IsConnected = false;
	//���������ʼ��ΪNULL
	m_result = NULL;
}

/*
    �������ܣ������������ر�MySQL���Ӳ��ͷŽ����
	��������
	����ֵ����
*/
Sql::~Sql()
{
    if (IsConnected)
	    Close();
	
	if (m_result)
	    FreeResult();
}

/*
    �������ܣ�����MySQL������
	��������
	����ֵ��
	    -1   ����ʧ��
		0    ���ӳɹ�
*/
int Sql::Connect()
{	
	//����Ѿ�������ֱ�ӷ���
	if (IsConnected)
	    return 0;
	
	string host, username, password, dbname, charset;
	//��ȡ�����ļ�ʧ�ܾͲ���Ĭ�ϵ�����
	if (-1 == read_config(m_filename, host, username, password, dbname, charset))
	{
	    	host = "127.0.0.1";
		username = "root";
		password = "";
		dbname = "rzxdb";
		charset = "utf8";
	}
	
	//���ӷ�����
	if (0 == Connect(host.c_str(), username.c_str(), password.c_str(), dbname.c_str()))
	{
	    IsConnected = true;
				
		//���ӷ������ɹ�֮�������ַ���
		char cmd[32] = {0};
		snprintf(cmd, sizeof(cmd) - 1, "set names %s", charset.c_str());	
		Execute(cmd);//ִ��SQL��䣬�����ַ���
		
		return 0;
	}
	else
	{
	    IsConnected = false;	
		return -1;
	}
}

/*
    �������ܣ�����MySQL
	������
	    host      ��������ַ
		username  �û���
		passwd    ����
		dbname    ���ݿ���
		port      �˿ں�
	����ֵ��
	    -1     ����ʧ��
		0      ���ӳɹ�
*/
int Sql::Connect(const char *host, const char *username, const char *passwd, const char *dbname, unsigned int port)
{
    if (NULL == host || NULL == username || NULL == passwd || NULL == dbname || port < 0)
	    return -1;

	//����MySQL������
	mysql_init(&m_connection);//��ʼ��MYSQL����
	int n;
    	n = 20; // �趨20�볬ʱ
    	mysql_options(&m_connection, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&n);
    	n = 1;  // �Ϻ��Զ�����
    	mysql_options(&m_connection, MYSQL_OPT_RECONNECT, (char *)&n);
	
	if (NULL != mysql_real_connect(&m_connection, host, username, passwd, dbname, port, NULL, CLIENT_FOUND_ROWS))
	    return 0;
	else
	    return -1;
}

/*
    �������ܣ��ر�MySQL����
	��������
	����ֵ����
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
    �������ܣ�ִ�в��롢���¡�ɾ���ȹ���
	������
	    cmd      insert��delete��update�Ȳ���Ҫ���ؽ������SQL���
	����ֵ��
	    0    �ɹ�
		��0  ʧ��
*/
int Sql::Execute(const char *cmd)
{
    if (NULL == cmd || !IsConnected)
	    return -1;
		
	//insert��delete��update�Ȳ��������ͷŽ����	
	return mysql_query(&m_connection, cmd);
}

/*
    �������ܣ�ִ�в�ѯ��䣬��ý����
	������
	    cmd      select���
	����ֵ��
	    0    �ɹ�
		-1   ʧ��
*/
int Sql::Query(const char *cmd)
{
    if (NULL == cmd || !IsConnected)
	    return -1;
	
	FreeResult();//�ͷŽ����
	
	if (mysql_query(&m_connection, cmd) < 0)//��ѯ
		return -1;
	
	m_result = mysql_store_result(&m_connection);//��������
	
	if (NULL == m_result)
	    return -1;
		
	return 0;
}

/*
    �������ܣ����ؽ�����е�����
	��������
	����ֵ��
	    -1     �����Ϊ�գ��޷��õ�����
		����   ������е�����
*/
int Sql::GetRowCount()
{
    //��������벻ΪNULL
	if (NULL != m_result)
	    return mysql_num_rows(m_result);
	else
	    return -1;
}

/*
    �������ܣ����ؽ�����е�����
	��������
	����ֵ��
	    -1     �����Ϊ�գ��޷��õ�����
		����   ������е�����
*/
int Sql::GetColCount()
{
    //��������벻ΪNULL
	if (NULL != m_result)
	    return mysql_num_fields(m_result);
	else
	    return -1;
}

/*
    �������ܣ��õ�������е�index�е�����
	������
	    index  ��ʾ��index�У���0��ʼ
	����ֵ��
	    NULL   �޷��õ�����
		����   ��index�е�����
*/
const char* Sql::GetColName(const int &index)
{
    //�ڽ������Ϊ�յ�����²�ȥ��ȡ����
	if (NULL != m_result)
	{
	    MYSQL_FIELD *field;
	    field = mysql_fetch_field_direct(m_result, index);
		if (NULL == field)//��ȡʧ�ܾ�ֱ�ӷ���NULL
		    return NULL;
		
		return field->name;
	}
	
	return NULL;
}

/*
    �������ܣ��õ�������е�һ��
	��������
	����ֵ��
	    NULL   �����Ϊ�գ��޷��õ���
		����   ������е�һ��
*/
MYSQL_ROW Sql::GetRow()
{
    //��������벻ΪNULL
	if (NULL != m_result)
	    return mysql_fetch_row(m_result);
	
	return NULL;
}

/*
    �������ܣ��õ���ǰ�������е����ֵ
	��������
	����ֵ��
	    -1    û������MySQL
	    ����  ��ǰ�������е����ֵ
*/
unsigned int Sql::GetInsertID()
{
    if (IsConnected)
        return (unsigned int)mysql_insert_id(&m_connection);
	else
	    return -1;
}

/*
 *  ���ܣ�����sql����ʱ�����ֶ��е�ת���ַ���'��"��\��
 *  ������field[in][out]--��Ҫ������ֶδ���ע�⣬�ڵ��ú󣬸ô������޸ģ�����
 *                        ����Ҫת���ַ�ʱ����ȷ������һ����д���ڴ棨�����飩��
 *        nMaxLen[in]--ָ��field����󳤶ȣ�
 *  ��д��Rocky 2008-10-27 11:38:52
 *  ���أ�field�ĵ�ַ��������ʱ������ԭfieldֵ�����򷵻ص�����ת��ģ���
 */
char *Sql::Escape(char field[], int nMaxLen)
{
    int len, count;
    char *p, *dest;

   // assert(NULL != field);

    // ɨ�贮����Ҫת����ַ�������������'��"��\��
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
    // û��ת���ַ�����ֱ�ӷ���ԭֵ��
    if(0 == count)
    {
        return field;
    }


    // �Ӻ�סǰɨ�裨������ԭ���в�����
    p = field + len;
    dest = field + len + count;

    // ������ֱ�ӷ��أ�
    if(dest - field > nMaxLen)
    {
        return field;
    }

    // ��ת��
    for(; p >= field; p--)
    {
        *dest-- = *p;
        // ���������ַ�����ǰ�����ת���ַ���
        if('\\' == *p || '\'' == *p || '"' == *p)
        {
            *dest-- = '\\';
        }
    }

    return field;
}

/*
    �������ܣ����ش�����Ϣ
	��������
	����ֵ��
	    ������ݿ��Ѿ������򷵻ش�����Ϣ
		���򣬷���NULL
*/
const char* Sql::Error()
{
    //������ݿ��Ѿ����ϣ��򷵻ش�����Ϣ
	if (IsConnected)
	    return mysql_error(&m_connection);
	
	return NULL;
}

/*
    �������ܣ��ͷŽ����
	��������
	����ֵ����
*/
void Sql::FreeResult()
{
    //����������ΪNULL�����ͷŽ�����������������ΪNULL
    if (NULL != m_result)
	{
	    mysql_free_result(m_result);
		m_result = NULL;
	}
}

/*
    �������ܣ�����MySQL�����Ǹ�C�ӿ�
	������
	    config_file   �����ļ�����ΪNULLʱ��Ĭ�ϵ������ļ�CONFIG_FILE_NAME
		db_connection ���ݿ�����
		sz_charset    �ַ��������Σ����������ļ��е��ַ���
		charset_len   charset�Ĵ�С
	����ֵ��
	    -1 ����ʧ��
		0  ���ӳɹ�
*/

int connect_mysql(const char *config_file, MYSQL *db_connection, char *sz_charset, int charset_len)
{
    if (NULL == sz_charset || charset_len < 0 || NULL == db_connection)
	    return -1;
	
	//�����ļ���
	string file_name;
	if (NULL == config_file)
	    file_name.assign(CONFIG_FILE_NAME);
	else
	    file_name.assign(config_file);
		
	string host, username, password, dbname, charset;
	//��ȡ�����ļ�����Ͳ���Ĭ������
	if (-1 == read_config(file_name, host, username, password, dbname, charset))
	{
	    host = "127.0.0.1";
		username = "root";
		password = "";
		dbname = "rzxdb";
		charset = "utf8";
	}
		
	//����MySQL���ݿ�
	mysql_init(db_connection);//��ʼ��MYSQL����
	if (mysql_real_connect(db_connection, host.c_str(), username.c_str(), password.c_str(), 
	                       dbname.c_str(), SQLPORT, NULL, CLIENT_FOUND_ROWS) == NULL)
	    return -1;
	
	//�����ַ���
	strncpy(sz_charset, charset.c_str(), charset_len - 1);
	return 0;
}


unsigned int Sql::ErrorNo()
{
		if (IsConnected)
	return mysql_errno(&m_connection); 
	return 0;
}

