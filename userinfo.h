/*
 * 用户的个人信息界面
 * 可以实现更改用户用户名，密码的功能
 */

#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class userInfo;
}

class userInfo : public QDialog
{
    Q_OBJECT

public:
    explicit userInfo(QWidget *parent = 0);
    ~userInfo();

private slots:
    void on_ChangeName_Btn_clicked();

    void on_ChangePwd_Btn_clicked();

    //void receivedb(QSqlDatabase db);

    void on_pushButton_clicked();

signals:
    void changetitle(QString);

private:
    Ui::userInfo *ui;

    QSqlDatabase database;

    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;

};

#endif // USERINFO_H
