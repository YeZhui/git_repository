#ifndef TIPSTHREAD_H
#define TIPSTHREAD_H

#include <QThread>
#include <QEventLoop>
#include <QTimer>
//#include <iostream>
//using namespace std;

class TipsThread : public QThread
{
    Q_OBJECT
public:
    explicit TipsThread();

    bool m_bStopThread; //结束线程标志位

    void run();

signals:
    void showDialog(QString p_qstrTips);
};

#endif // TIPSTHREAD_H
