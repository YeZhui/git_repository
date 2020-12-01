#include "setTimeDialog.h"
#include "ui_setTimeDialog.h"
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "tipsSql.h"
#include <QMessageBox>

SetTimeDialog::SetTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTimeDialog)
{
    ui->setupUi(this);
    QDateTime curDateTime = QDateTime::currentDateTime();
    ui->timeEdit->setTime(curDateTime.time());
}

SetTimeDialog::~SetTimeDialog()
{
    delete ui;
}

QString SetTimeDialog::GetTime()
{
    QTime showTime = ui->timeEdit->time();
    QString strTime = showTime.toString();
    return strTime;
}

QString SetTimeDialog::GetTips()
{
    return ui->lineEdit->displayText();
}

int SetTimeDialog::GetAbleFlag()
{
    return ui->comboBox->currentIndex();
}

void SetTimeDialog::on_buttonBox_accepted()
{
    CTipsSql *pTipsSql = CTipsSql::getInstance();
    int iErrorCode = 0;
    iErrorCode = pTipsSql->Insert(GetTime(), GetTips(), GetAbleFlag());
    if ( iErrorCode == -1 )
    {
        QMessageBox::critical(this, "错误提示", pTipsSql->GetErrorMsg());
    }
}
