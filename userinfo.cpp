#include "userinfo.h"
#include "ui_userinfo.h"

userInfo::userInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle());

    ui->label->setPixmap(QPixmap(":/img/cat.png").scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
    ui->userName_show->setText(parent->windowTitle());
}

userInfo::~userInfo()
{
    delete ui;
}
