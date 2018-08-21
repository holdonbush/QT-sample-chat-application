#include "loginin.h"
#include "ui_loginin.h"
#include "register.h"
#include "widget.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

LoginIn::LoginIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginIn)
{
    ui->setupUi(this);
    setWindowTitle("登陆");

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
}

void LoginIn::on_Login_Btn_clicked()
{
    if(matchFlag == false)
    {
        //用户名错误
        qDebug()<<"name invalid";
        ui->label_2->setText("用户名错误");
    }
    else
    {
        ui->label_2->setText("");
        if(usr_passwd!=ui->PWD_LineEdit->text())
        {
            //密码错误
            qDebug()<<"passwd not match";
            ui->label_3->setText("密码错误");
        }
        else
        {
            ui->label_3->setText("");
            //用户名和密码均正确
            this->hide();
            emit firstpageshow(usr_name);
        }
    }
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
