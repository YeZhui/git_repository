#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QHideEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionTimeSet_triggered();

    void on_actionShowTime_triggered();

    void on_showTips(QString p_qstrTips);

    void on_trayActivated(QSystemTrayIcon::ActivationReason p_ireason);

    void on_actionModel_triggered();

private:
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);


private:
    Ui::MainWindow *ui;
    QMenu *m_TrayMenu;
    QSystemTrayIcon *m_Tray;
};

#endif // MAINWINDOW_H
