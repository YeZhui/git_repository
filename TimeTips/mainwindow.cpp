#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "setTimeDialog.h"
#include "showTipsWidget.h"
#include "tipsSql.h"
#include <tipsDialog.h>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CTipsSql *pTipsSql = CTipsSql::getInstance();
    int iErrorCode = 0;
    iErrorCode = pTipsSql->ConnectDb();
    if ( iErrorCode == -1 )
    {
        QMessageBox::critical(this, "错误提示", pTipsSql->GetErrorMsg());
    }
    iErrorCode = pTipsSql->CreateTable();
    if ( iErrorCode == -1 )
    {
        QMessageBox::critical(this, "错误提示", pTipsSql->GetErrorMsg());
    }

    //设置任务栏图标、提示信息
    m_Tray = new QSystemTrayIcon(this);
    m_Tray->setIcon(QIcon(":/favicon32.ico"));
    m_Tray->setToolTip("桌面小闹钟");
    m_Tray->show();
    m_Tray->showMessage("桌面小闹钟", "桌面小闹钟", QSystemTrayIcon::Information, 5000);

    //设置任务栏右键菜单和响应槽函数
    QAction *minAction = new QAction("最小化", this);
    connect(minAction, SIGNAL(triggered()), this, SLOT(hide()));
    QAction *maxAction = new QAction("最大化", this);
    connect(maxAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    QAction *restoreAction = new QAction("恢复", this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction *quitAction = new QAction("退出", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_TrayMenu = new QMenu(this);
    m_TrayMenu->addAction(minAction);
    m_TrayMenu->addAction(maxAction);
    m_TrayMenu->addAction(restoreAction);
    m_TrayMenu->addSeparator();
    m_TrayMenu->addAction(quitAction);
    m_Tray->setContextMenu(m_TrayMenu);
    //定义任务栏鼠标事件
    connect(m_Tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_trayActivated(QSystemTrayIcon::ActivationReason)));

    //on_showTips("初始化测试");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QString aboutTitle = "关于";
    QString strInfo = "作者：晟夏的叶\n版本：V1.0\n保留所有版权";
    QMessageBox::about(this, aboutTitle, strInfo);
}

void MainWindow::on_actionTimeSet_triggered()
{
    SetTimeDialog setTimeDialog(this);
    setTimeDialog.exec();
}

void MainWindow::on_actionShowTime_triggered()
{
    ShowTipsWidget *widget = new ShowTipsWidget();
    widget->setWindowModality(Qt::NonModal);
    widget->show();
}

void MainWindow::on_showTips(QString p_qstrTips)
{
    CTipsDialog *tipsDialog = new CTipsDialog;
    tipsDialog->SetTips(p_qstrTips);
    tipsDialog->exec();
}

void MainWindow::on_trayActivated(QSystemTrayIcon::ActivationReason p_ireason)
{
    switch(p_ireason)
    {
    case QSystemTrayIcon::Trigger:
        this->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if ( m_Tray->isVisible())
    {
        hide();
        event->ignore();
    }
}
void MainWindow::hideEvent(QHideEvent *event)
{
    if ( m_Tray->isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainWindow::on_actionModel_triggered()
{
    on_showTips("这里是模拟弹窗");
}
