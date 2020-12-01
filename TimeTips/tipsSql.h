#ifndef TIPSSQL_H
#define TIPSSQL_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>

class CTipsSql
{
public:
    static CTipsSql* getInstance();//单例模式
    QString GetErrorMsg();
    int ConnectDb();
    int CreateTable();
    int Insert(QString p_qstrTime, QString p_qstrTips, int p_iFlag);
    int Update(int p_iTipsNo, QString p_qstrTipsTime, QString p_qstrDisplay, int p_iFlag);
    int Delete(int p_iTipsNo);
    QSqlQuery Selete(QString p_qstrSql);

private:
    CTipsSql();
    CTipsSql(const CTipsSql&);
    CTipsSql& operator=(const CTipsSql&);
    ~CTipsSql();

    class CDelTipsSql
    {
    public:
        ~CDelTipsSql()
        {
            if ( CTipsSql::instance)
                delete CTipsSql::instance;
        }
    };

    static CTipsSql* instance;
    QString m_qstrErrorMsg;
    static CDelTipsSql delTipsSql; //定义一个静态成员变量，程序结束时，系统会自动调拥它的析构函数
    QSqlDatabase m_sqlDataBase;
};

#endif // TIPSSQL_H
