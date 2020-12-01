#ifndef SETTIMEDIALOG_H
#define SETTIMEDIALOG_H

#include <QDialog>
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class SetTimeDialog;
}

class SetTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetTimeDialog(QWidget *parent = nullptr);
    ~SetTimeDialog();
    QString GetTime();
    QString GetTips();
    int GetAbleFlag();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetTimeDialog *ui;
    //timeEdit lineEdit comboBox
};

#endif // SETTIMEDIALOG_H
