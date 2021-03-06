#include "register.h"
#include "ui_register.h"
#include "loginin.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowTitle("注册");
    setWindowIcon(QPixmap(":/img/register.png"));
    ui->PWD_LineEdit->setEchoMode(QLineEdit::Password);
    ui->PWDConf_LineEdit->setEchoMode(QLineEdit::Password);

    //建立连接
    status = false;
    port = 8010;
    QString ip = "127.0.0.1";
    serverIP = new QHostAddress;
    serverIP->setAddress(ip);
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(*serverIP,port);

    connect((LoginIn* )parent,SIGNAL(transmitdb(QSqlDatabase)),this,SLOT(receivedb(QSqlDatabase)));

}

Register::~Register()
{
    delete ui;
}

//数据库相关操作语句
QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30))";
QString select_max_sql = "select max(chatid) from user";
QString insert_sql = "insert into user values (?, ?, ?)";
QString select_sql = "select name from user";

/*
 * 函数名： on_Back_Btn_clicked
 * 功能：返回登陆页面
 * 参数：NULL
 * 返回值：void
 */
void Register::on_Back_Btn_clicked()
{
    this->close();
}

/*
 * 函数名：on_Registe_Btn_clicked
 * 功能：用户注册，并向服务端发送注册信息，服务端将信息储存到数据库，
 *      本地也会建立一个数据库，仅仅只是存储，验证登陆是从服务端的
 *      数据库获取信息来验证
 * 参数：NULL
 * 返回值：void
 */
void Register::on_Registe_Btn_clicked()
{
    bool nameFlag = false;
    bool passwdFlag = false;
    int newchatid = max_id+1;
    QString newpasswd = NULL;
    QString newname = NULL;
    //注册时的信息填写验证
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

    //将用户名与密码发送至服务端，用于保存至服务端的数据库中
    QString str = ui->UserName_LineEdit->text()+"/" + ui->PWD_LineEdit->text();
    tcpSocket->write(str.toLatin1());
    tcpSocket->flush();

    tcpSocket->disconnectFromHost();
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
