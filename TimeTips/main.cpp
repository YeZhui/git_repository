#include "mainwindow.h"
#include <QApplication>
#include "tipsThread.h"
#include <QObject>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    TipsThread *thread = new TipsThread();
    QObject::connect(thread, SIGNAL(showDialog(QString)), &w, SLOT(on_showTips(QString)));
    thread->start();//启动线程

    return a.exec();
}
