#include "tipsThread.h"
#include "tipsSql.h"
#include <QSqlQuery>
#include <QTime>
#include <QVariant>
#include <QDebug>

TipsThread::TipsThread()
{
    m_bStopThread = false;
}

void TipsThread::run()
{
    m_bStopThread = false;
    qDebug() << "启动线程";
    bool bFlag = false;
    int iSleep = 500;
    while( !m_bStopThread )
    {
        //遍历校验闹钟
        QSqlQuery sql_query;
        CTipsSql* pTipsSql = CTipsSql::getInstance();
        sql_query = pTipsSql->Selete("select * from TimeTip where flag = 0");//只查询生效闹钟
        if ( sql_query.isSelect() )
        {
            while(sql_query.next())
            {
                //获取当前时间，得到hh:mm:ss格式
                QTime curTime = QTime::currentTime();
                QString strCurTime = curTime.toString(Qt::TextDate);
                //qDebug() << "当前时间" << strCurTime;
                //qDebug() << "闹钟时间" << sql_query.value(1).toString();

                if ( sql_query.value(1).toString() == strCurTime )
                {
                    //QMessageBox::Information()
                    emit showDialog(sql_query.value(2).toString());
                    bFlag = true; //有闹钟匹配到
                }
            }
        }
        else {
            //QMessageBox::critical(this, "错误提示", pTipsSql->GetErrorMsg());
        }

        if ( bFlag == true)
        {
            bFlag = false;
            iSleep = 1000;
        }
        else {
            iSleep = 500;
        }

        QEventLoop eventLoop;
        QTimer::singleShot(iSleep, &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
}
