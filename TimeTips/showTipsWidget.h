#ifndef SHOWTIPSWIDGET_H
#define SHOWTIPSWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QComboBox>

namespace Ui {
class ShowTipsWidget;
}

class ShowTipsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowTipsWidget(QWidget *parent = nullptr);
    ~ShowTipsWidget();

private slots:
    void on_updButton_clicked();

    void on_closeButton_clicked();

    void on_delButton_clicked();

private:
    Ui::ShowTipsWidget *ui;
    void DisplayTips();
    QTableWidgetItem *item;
    QComboBox *comboBox;
};

#endif // SHOWTIPSWIDGET_H
