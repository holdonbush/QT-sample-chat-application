#include "loginin.h"
#include "ui_loginin.h"
#include "register.h"
#include "widget.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QLineEdit>

LoginIn::LoginIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginIn)
{
    ui->setupUi(this);
    setWindowTitle("登陆");
    ui->PWD_LineEdit->setEchoMode(QLineEdit::Password);
    ui->label_2->clear();
    ui->label_3->clear();

    status = false;
    port = 8010;
    QString ip = "127.0.0.1";
    serverIP = new QHostAddress;
    serverIP->setAddress(ip);
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(*serverIP,port);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(datareceived()));

    connect(ui->UserName_LineEdit,SIGNAL(textChanged(QString)),this,SLOT(getUserInfo(QString)));
    tableFlag = false;

    //创建SQLITE数据库
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("database.db");

    //打开数据库
    if(!database.open()){
        qDebug()<<database.lastError();
        qFatal("failed to connect.") ;
    }
    else
    {
        qDebug()<<"open seccess";
        QSqlQuery sql_query;        //改变量必须在成功打开数据库后定义才有效
        sql_query.prepare(select_table);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            QString tableName;
            while(sql_query.next())
            {
                tableName = sql_query.value(0).toString();
                qDebug()<<tableName;
                //数据库表不存在
                if(tableName.compare("user"))
                {
                    tableFlag=false;
                    qDebug()<<"table is not exist";
                }
                //数据库表已存在
                else
                {
                    tableFlag=true;
                    qDebug()<<"table is exist";
                }
            }
        }
        if(tableFlag==false)
        {
            //创建名为user的数据库表
            sql_query.prepare(create_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                qDebug()<<"table created!";
            }
        }
    }
}

LoginIn::~LoginIn()
{
    delete ui;
}

void LoginIn::on_Registe_Btn_clicked()
{
    Register r;
    this->close();
    emit transmitdb(database);
    r.exec();
    this->show();
    this->ui->label_2->clear();
    this->ui->label_3->clear();
}

/*
 * 函数名：on_Login_Btn_clicked
 * 功能：向服务端发送用户名密码信息，服务端进行验证正确性
 * 返回值：void
 */
void LoginIn::on_Login_Btn_clicked()
{
    QString name1 = ui->UserName_LineEdit->text()+"/-";
    qDebug()<<"click";
    qDebug()<<matchFlag;


    qDebug()<<"1";
    tcpSocket->write(name1.toLocal8Bit());
    tcpSocket->flush();
}

void LoginIn::getUserInfo(QString name)
{
    QSqlQuery sql_query;
    QString tempstring = "select * from user where name='"+name+"'";
    qDebug()<<sql_query.lastError();
    if(!sql_query.exec(tempstring))
    {
        qDebug()<<sql_query.lastError();
        matchFlag = false;
    }
    else
    {
        while(sql_query.next())
        {
            usr_id = sql_query.value(0).toInt();
            usr_passwd = sql_query.value(1).toString();
            usr_name = sql_query.value(2).toString();

            qDebug()<<QString("charid = %1   passwd=%2   name=%3").arg(usr_id).arg(usr_passwd).arg(usr_name);

        }
        if(usr_name==name) matchFlag=true;
        else               matchFlag=false;
    }

}

/*
 * 函数名：datareceived
 * 功能：从服务端接收传回来的数据，用于验证用户名密码的信息是否正确
 * 返回值：void
 */
void LoginIn::datareceived()
{
    qDebug()<<"receive function";
    QByteArray buffer;
    buffer = tcpSocket->readAll();
    QString str = QString::fromLocal8Bit(buffer);
    qDebug()<<str;
    ui->label_2->clear();
    ui->label_3->clear();
    if(str != "***")
    {
        if(str!=ui->PWD_LineEdit->text())
        {
            qDebug()<<"passwd not match";
            ui->label_3->setText("密码错误");
            ui->UserName_LineEdit->clear();
            ui->PWD_LineEdit->clear();
        }
        else
        {
            ui->label_3->clear();
            this->close();
            emit firstpageshow(ui->UserName_LineEdit->text());
        }
    }
    else if(str.contains("*")) {
        //ui->label_2->setText("用户名错误");
        QMessageBox::warning(this,tr("ERROR"),tr("用户名错误"));
        ui->UserName_LineEdit->clear();
        ui->PWD_LineEdit->clear();
    }
    else
    {};
}

void LoginIn::showThisPage()
{
    ui->UserName_LineEdit->clear();
    ui->PWD_LineEdit->clear();
    this->exec();
}
