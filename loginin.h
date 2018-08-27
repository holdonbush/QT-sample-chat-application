/*
 * IM的登陆界面
 */

#ifndef LOGININ_H
#define LOGININ_H

#include <QDialog>
#include <QPainter>
#include <QtSql/QSqlDatabase>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QHostAddress>

namespace Ui {
class LoginIn;
}

class LoginIn : public QDialog
{
    Q_OBJECT

public:
    explicit LoginIn(QWidget *parent = 0);
    ~LoginIn();

private slots:
    void on_Registe_Btn_clicked(); //登陆界面注册按钮槽函数

    void on_Login_Btn_clicked(); //登陆界面登陆按钮槽函数

    void getUserInfo(QString name);   //获取用户信息，用以验证

    void datareceived();

    void showThisPage();

signals:
    void firstpageshow(QString); //转到第一个界面

    void transmitdb(QSqlDatabase);

private:
    Ui::LoginIn *ui;
    bool status;
    int port;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QHostAddress *serverIP;


    QSqlDatabase database;     //定义数据库对象
    bool tableFlag;

    int usr_id;
    QString usr_passwd;
    QString usr_name;

    bool matchFlag;

    //定义数据库要执行的语句
    QString select_table = "select tbl_name name from sqlite_master where type = 'table'";
    QString create_sql = "create table user (chatid int primary key, passwd varchar(30), name varchar(30))";
    QString select_max_sql = "select max(chatid) from user";
    QString insert_sql = "insert into user values (?, ?, ?)";
    QString select_sql = "select name from user";

    QString select_nameInfo = "selcet * from user where name=";


};

#endif // LOGININ_H
