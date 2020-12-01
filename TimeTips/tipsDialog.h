#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class CTipsDialog;
}

class CTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTipsDialog(QWidget *parent = nullptr);
    ~CTipsDialog();
    void SetTips(QString p_qstrTips);

private:
    Ui::CTipsDialog *ui;
};

#endif // TIPSDIALOG_H
