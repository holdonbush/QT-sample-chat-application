#include "userinfo.h"
#include "ui_userinfo.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

userInfo::userInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle());

    ui->label->setPixmap(QPixmap(":/img/cat.png").scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
    ui->userName_show->setText(parent->windowTitle());
    ui->lineEdit->setText("****");
}

userInfo::~userInfo()
{
    delete ui;
}

void userInfo::on_ChangeName_Btn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,tr("更改姓名"),tr("请输入姓名"),QLineEdit::Normal,ui->userName_show->text(),&ok);
    if(ok&&!text.isEmpty())
    {
        ui->userName_show->setText(text);
    }
}

void userInfo::on_ChangePwd_Btn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,tr("更改密码"),tr("请输入密码"),QLineEdit::Password,ui->lineEdit->text(),&ok);
    qDebug()<<text;
    if(ok&&!text.isEmpty())
    {
        QMessageBox::information(this,tr("information"),tr("修改成功"),QMessageBox::Ok);
    }
}
