#include "showTipsWidget.h"
#include "ui_showTipsWidget.h"
#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include "tipsSql.h"
#include <QStringList>

ShowTipsWidget::ShowTipsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowTipsWidget)
{
    ui->setupUi(this);
    DisplayTips();
    item = nullptr;
    comboBox = nullptr;
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);//让table失去默认焦点
}

ShowTipsWidget::~ShowTipsWidget()
{
    delete ui;
    if (item != nullptr )
        delete item;
    if (comboBox != nullptr )
        delete comboBox;
}

void ShowTipsWidget::DisplayTips()
{
    ui->tableWidget->clearContents();//只清除内容，不清除表头
    ui->tableWidget->setRowCount(0);
    int curRow = ui->tableWidget->currentRow();

    //comboBox = new QComboBox();
    QStringList strList;
    strList << "生效" << "不生效";

    QSqlQuery sql_query;
    CTipsSql* pTipsSql = CTipsSql::getInstance();
    sql_query = pTipsSql->Selete("select * from TimeTip");
    if ( sql_query.isSelect() )
    {
        while(sql_query.next())
        {
            curRow++;
            //插入一行
            ui->tableWidget->insertRow(curRow);

            item = new QTableWidgetItem(sql_query.value(0).toString());
            item->setBackground(QBrush(QColor(Qt::lightGray))); //设置第一列背景色为灰色
            item->setFlags(item->flags() & (~Qt::ItemIsEditable)); //设置第一列不可编辑
            ui->tableWidget->setItem(curRow, 0, item);

            //使用时间控件
            item = new QTableWidgetItem(sql_query.value(1).toString());
            ui->tableWidget->setItem(curRow, 1, item);
            //connect(item, SIGNAL(itemChanged(item)), this, SLOT(SetCurrentRow()));

            item = new QTableWidgetItem(sql_query.value(2).toString());
            ui->tableWidget->setItem(curRow, 2, item);

            //使用combo box
            comboBox = new QComboBox();
            comboBox->addItems(strList);
            int iFlag = sql_query.value(3).toInt();
            if ( iFlag == 0)
            {
                comboBox->setCurrentIndex(0);
            }
            else {
                comboBox->setCurrentIndex(1);
            }
            ui->tableWidget->setCellWidget(curRow, 3, comboBox);
        }
    }
    else {
        QMessageBox::critical(this, "错误提示", pTipsSql->GetErrorMsg());
    }
}

//更新某条记录
void ShowTipsWidget::on_updButton_clicked()
{
    //先获取当前选择的记录，如不存在，则弹窗报错
    int iRow = ui->tableWidget->currentRow();
    if ( iRow == -1 )
    {
        QMessageBox::critical(this, "错误提示", "请选中一行");
        return;
    }
    int iFlag = 0;

    //获取闹钟编号
    item = ui->tableWidget->item(iRow, 0);
    QString strTipsNo = item->text();
    int iTipsNo = strTipsNo.toInt();

    //获取时间
    item = ui->tableWidget->item(iRow, 1);
    QString strTipsTime = item->text();

    //获取提醒事项
    item = ui->tableWidget->item(iRow, 2);
    QString strDisplay = item->text();

    //获取生效标志
    item = ui->tableWidget->item(iRow, 3);
    QString strFlag = item->text();
    qDebug() << strFlag;
    if (strFlag == "生效")
    {
        iFlag = 0;
    }
    else {
        iFlag = 1;
    }
    //界面上修改以后，更新至数据库
    int iErrCode = CTipsSql::getInstance()->Update(iTipsNo, strTipsTime, strDisplay, iFlag);
    if ( iErrCode == -1 )
    {
        QMessageBox::critical(this, "错误提示", CTipsSql::getInstance()->GetErrorMsg());
    }
    else {
        QMessageBox::information(this, "提示", "信息修改成功");
        DisplayTips();
    }
}

void ShowTipsWidget::on_closeButton_clicked()
{
    this->close();
}

void ShowTipsWidget::on_delButton_clicked()
{
    //先获取当前选择的记录，如不存在，则弹窗报错
    int iRow = ui->tableWidget->currentRow();
    if ( iRow == -1 )
    {
        QMessageBox::critical(this, "错误提示", "请选中一行");
        return;
    }

    //获取闹钟编号
    item = ui->tableWidget->item(iRow, 0);
    QString strTipsNo = item->text();
    int iTipsNo = strTipsNo.toInt();

    //界面上修改以后，更新至数据库
    int iErrCode = CTipsSql::getInstance()->Delete(iTipsNo);
    if ( iErrCode == -1 )
    {
        QMessageBox::critical(this, "错误提示", CTipsSql::getInstance()->GetErrorMsg());
    }
    else {
        QMessageBox::information(this, "提示", "信息删除成功");
        DisplayTips();
    }
}
