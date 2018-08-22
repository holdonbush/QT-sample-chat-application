#include "register.h"
#include "ui_register.h"
#include "loginin.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowTitle("注册");
    ui->PWD_LineEdit->setEchoMode(QLineEdit::Password);
    ui->PWDConf_LineEdit->setEchoMode(QLineEdit::Password);

    connect((LoginIn* )parent,SIGNAL(transmitdb(QSqlDatabase)),this,SLOT(receivedb(QSqlDatabase)));

}

Register::~Register()
{
    delete ui;
}

QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30))";
QString select_max_sql = "select max(chatid) from user";
QString insert_sql = "insert into user values (?, ?, ?)";
QString select_sql = "select name from user";

void Register::on_Back_Btn_clicked()
{
    this->close();
}

void Register::on_Registe_Btn_clicked()
{
    bool nameFlag = false;
    bool passwdFlag = false;
    int newchatid = max_id+1;
    QString newpasswd = NULL;
    QString newname = NULL;

    if(ui->PWD_LineEdit->text()==""||ui->PWDConf_LineEdit->text()=="")
    {
        passwdFlag = false;
        ui->label_3->setText("请输入密码");
        ui->label_4->setText("请输入密码");
    }
    else if(ui->PWD_LineEdit->text()==ui->PWDConf_LineEdit->text())
    {
        ui->label_3->setText("");
        ui->label_4->setText("");
        passwdFlag=true;
    }
    else
    {
        ui->label_3->setText("密码不匹配");
        ui->label_4->setText("密码不匹配");
        qDebug()<<"passwd err";
        passwdFlag=false;
    }

    //数据库操作
    QSqlQuery sql_query;

    //查询最大id
    max_id = 0;
    sql_query.prepare(select_max_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else {
        while (sql_query.next()) {
            max_id = sql_query.value(0).toInt();
            qDebug()<<QString("max chatid:%1").arg(max_id);

        }
    }

    //查询部分数据
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(1)
        {
            if(sql_query.next())    //name有数据
            {
                QString name = sql_query.value("name").toString();
                qDebug()<<QString("name=%1").arg(name);

                if(ui->UserName_LineEdit->text()==name)
                {
                    qDebug()<<"name existed";
                    ui->label_2->setText("用户名已存在");
                    nameFlag=false;
                    break;
                }
                else
                {
                    ui->label_2->setText("");
                    nameFlag=true;
                }
            }
            else {
                nameFlag = true;
                break;
            }
        }
    }

    newchatid = max_id+1;
    if(nameFlag==true)
        newname = ui->UserName_LineEdit->text();
    else {
        return;
    }
    if(passwdFlag==true)
        newpasswd = ui->PWD_LineEdit->text();
    else
        return;

    //插入数据
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(newchatid);
    sql_query.addBindValue(newpasswd);
    sql_query.addBindValue(newname);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else {
        qDebug()<<"inserted!";    //成功插入
    }

    this->close();
}

void Register::receivedb(QSqlDatabase db)
{
    qDebug()<<"receive db";
    database=db;
    if(!database.isOpen())
    {
        if(!database.open())
        {
            qDebug()<<database.lastError();
            qFatal("failed to connect");
            return;
        }
        else {

        }
    }
}
