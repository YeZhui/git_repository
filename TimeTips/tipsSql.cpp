#include "tipsSql.h"
//#include <qdebug.h>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#define DB_NAME "TimeTips.db"
#define TABLE_NAME1 "TimeTip"

CTipsSql::CTipsSql()
{
    m_qstrErrorMsg = "";
}

CTipsSql::~CTipsSql()
{
    //数据库如果是开着的，则关闭连接
    if ( m_sqlDataBase.isOpen())
        m_sqlDataBase.close();
}

CTipsSql::CTipsSql(const CTipsSql&)
{
}

CTipsSql& CTipsSql::operator=(const CTipsSql&)
{
    return *instance;
}

CTipsSql* CTipsSql::instance = new CTipsSql();
CTipsSql* CTipsSql::getInstance()
{
    return instance;
}

QString CTipsSql::GetErrorMsg()
{
    return m_qstrErrorMsg;
}

int CTipsSql::ConnectDb()
{
    m_sqlDataBase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDataBase.setDatabaseName(DB_NAME);
    if (!m_sqlDataBase.open())
    {
        m_qstrErrorMsg = QString("Error: Failed to connect database %1.").arg(m_sqlDataBase.lastError().text());
        return -1;
    }
    else
    {
        return 0;
    }
}

int CTipsSql::CreateTable()
{
    //检查表是否已经存在
    QSqlQuery sqlQuery;
    QString sql = QString("select * from sqlite_master where name='%1'").arg(TABLE_NAME1);
    sqlQuery.exec(sql);
    //如果表不存在则创建表格
    if ( !sqlQuery.next())
    {
        //创建表格
        if(!sqlQuery.exec("create table TimeTip(tipsNo INTEGER PRIMARY KEY AUTOINCREMENT, tipsTime text, display text, flag int)"))
        {
            m_qstrErrorMsg = QString("Error: Fail to create table %1.").arg(sqlQuery.lastError().text());
            return -1;
        }
    }
    return 0;
}

int CTipsSql::Insert(QString p_qstrTime, QString p_qstrTips, int p_iFlag)
{
    QSqlQuery sqlQuery;
    QString strSql;
    strSql = QString("INSERT INTO TimeTip(tipsTime, display, flag) VALUES( '%1', '%2', %3)").arg(p_qstrTime).arg(p_qstrTips).arg(p_iFlag);
    if(!sqlQuery.exec(strSql))
    {
        //qDebug() << sqlQuery.lastError();
        m_qstrErrorMsg = QString("Error:sql[%1] exec failed,msg[%2]").arg(strSql).arg(sqlQuery.lastError().text());
        return -1;
    }
    else
    {
        //qDebug() << "inserted Wang!";
        return 0;
    }
}

int CTipsSql::Update(int p_iTipsNo, QString p_qstrTipsTime, QString p_qstrDisplay, int p_iFlag)
{
    QSqlQuery sqlQuery;
    QString strSql;
    strSql = QString("update TimeTip set tipsTime='%1', display='%2', flag = %3 where tipsNo = %4").arg(p_qstrTipsTime).arg(p_qstrDisplay).arg(p_iFlag).arg(p_iTipsNo);
    if(!sqlQuery.exec(strSql))
    {
        //qDebug() << sqlQuery.lastError();
        m_qstrErrorMsg = QString("Error:sql[%1] exec failed,msg[%2]").arg(strSql).arg(sqlQuery.lastError().text());
        return -1;
    }
    else
    {
        //qDebug() << "inserted Wang!";
        return 0;
    }
}
int CTipsSql::Delete(int p_iTipsNo)
{
    QSqlQuery sqlQuery;
    QString strSql;
    strSql = QString("delete from TimeTip where tipsNo = %1").arg(p_iTipsNo);
    if(!sqlQuery.exec(strSql))
    {
        m_qstrErrorMsg = QString("Error:sql[%1] exec failed,msg[%2]").arg(strSql).arg(sqlQuery.lastError().text());
        return -1;
    }
    else
    {
        return 0;
    }
}

QSqlQuery CTipsSql::Selete(QString p_qstrSql)
{
    QSqlQuery sqlQuery;
    sqlQuery.exec(p_qstrSql);
    if ( !sqlQuery.exec())
    {
        //qDebug()<<sqlQuery.lastError();
        m_qstrErrorMsg = QString("Error:sql[%1] exec failed,msg[%2]").arg(p_qstrSql).arg(sqlQuery.lastError().text());
        return QSqlQuery(nullptr);
    }
    return sqlQuery;
}

