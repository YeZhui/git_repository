#include "tipsDialog.h"
#include "ui_tipsDialog.h"

CTipsDialog::CTipsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTipsDialog)
{
    ui->setupUi(this);
    //设置窗口总在最前
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    //设置背景色
    QColor color;
    color.setRgb(204, 232, 207);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, color);
    this->setPalette(pal);

    //设置label显示图片
    QImage image(":/baobao.JPG");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    //ui->imageLabel->setGeometry(0, 0, image.width(), image.height());

}

CTipsDialog::~CTipsDialog()
{
    delete ui;
}

void CTipsDialog::SetTips(QString p_qstrTips)
{
    QString qstrTips = "宝宝提醒:";
    qstrTips += p_qstrTips;
    ui->tipsLabel->setText(qstrTips);

    //设置字体颜色
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->tipsLabel->setPalette(pa);
    ui->tipsLabel->adjustSize(); //自适应
    ui->tipsLabel->setWordWrap(true);
    ui->tipsLabel->setAlignment(Qt::AlignTop);

    //设置字体大小
    //QFont ft;
    //ft.setPointSize(16);
    //ui->tipsLabel->setFont(ft);
}
