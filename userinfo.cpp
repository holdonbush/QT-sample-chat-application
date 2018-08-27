#include "userinfo.h"
#include "ui_userinfo.h"
#include "loginin.h"
#include "drawer.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDebug>

userInfo::userInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle());


    port = 8010;
    QString ip = "127.0.0.1";
    serverIP = new QHostAddress;
    serverIP->setAddress(ip);
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(*serverIP,port);

    ui->label->setPixmap(QPixmap(":/img/cat.png").scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
    ui->userName_show->setText(parent->windowTitle());
    ui->lineEdit->setText("****");

    connect(this,SIGNAL(changetitle(QString)),(Drawer*)parent,SLOT(changetitle(QString)));

    //
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("database.db");

    if(!database.open())
    {
        qDebug()<< database.lastError();
        qFatal("Faild to connect");
    }
    else
    {
        qDebug()<<"open success";
        QSqlQuery sqlquery(database);

        QString select_sql = "select chatid , passwd , name from user";
        if(!sqlquery.exec(select_sql))
        {
            qDebug()<<sqlquery.lastError();
        }
        else {
            while (sqlquery.next()) {
                //int id = sqlquery.value(0).toInt();
                //QString pwd = sqlquery.value(1).toString();
                //QString name = sqlquery.value(2).toString();
                if(sqlquery.value(2).toString() == windowTitle())
                    qDebug()<<sqlquery.value(0).toInt();
                //qDebug()<< QString("id:%1   name:%2    pwd:%3").arg(id).arg(name).arg(pwd);
            }
        }
    }
}

userInfo::~userInfo()
{
    delete ui;
}

/*
 * 函数名：on_ChangeName_Btn_clicked
 * 功能：更改用户名，向服务端发送新用户名，服务端在数据库相应位置更新信息
 * 返回值：void
 */
void userInfo::on_ChangeName_Btn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,tr("更改姓名"),tr("请输入姓名"),QLineEdit::Normal,ui->userName_show->text(),&ok);
    QSqlQuery sqlquery(database);

    QString change = windowTitle() + "@" + text;
    tcpSocket->write(change.toLocal8Bit());
    tcpSocket->flush();
    QString select_sql = "select chatid , passwd , name from user";
    QString update_sql = "update user set name = :name where chatid = :chatid ";

    if(!sqlquery.exec(select_sql))
    {
        qDebug()<<sqlquery.lastError();
    }
    else {
        while (sqlquery.next()) {
            if(sqlquery.value(2).toString() == windowTitle())
            {
                qDebug()<<sqlquery.value(0).toInt();
                int id = sqlquery.value(0).toInt();
                if(ok&&!text.isEmpty())
                {
                    sqlquery.prepare(update_sql);
                    sqlquery.bindValue(":name",text);
                    sqlquery.bindValue(":chatid",id);
                    if(!sqlquery.exec())
                    {
                        qDebug()<<sqlquery.lastError();
                    }
                    else
                    {
                        qDebug()<< "updated!";
                        ui->userName_show->setText(text);
                        setWindowTitle(text);
                        emit changetitle(text);
                    }
                }
            }
        }
    }

}

/*
 * 函数名：on_ChangePwd_Btn_clicked
 * 功能：更改密码，向服务端发送新密码，服务端在数据库相应位置更新信息
 * 返回值：void
 */
void userInfo::on_ChangePwd_Btn_clicked()
{
    bool ok;
    QString name = windowTitle();
    QString text = QInputDialog::getText(this,tr("更改密码"),tr("请输入密码"),QLineEdit::Password,ui->lineEdit->text(),&ok);
    qDebug()<<text;

    QString update_sql = "update user set passwd = :passwd where name = :name ";
    QSqlQuery sqlquery(database);
    if(ok&&!text.isEmpty())
    {
        sqlquery.prepare(update_sql);
        sqlquery.bindValue(":passwd",text);
        sqlquery.bindValue(":name",name);
        if(!sqlquery.exec())
        {
            qDebug()<<sqlquery.lastError();
        }
        else
        {
            qDebug()<< "updated!";
            QMessageBox::information(this,tr("information"),tr("修改成功"),QMessageBox::Ok);
        }
    }

    //
    QString change = name + "#" + text;
    tcpSocket->write(change.toLatin1());
    tcpSocket->flush();
}

/*
 * 函数名：on_pushButton_clicked
 * 功能：关闭该页面
 * 返回值：void
 */
void userInfo::on_pushButton_clicked()
{
    this->close();
}
