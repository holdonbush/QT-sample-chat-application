#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QHostAddress>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_Back_Btn_clicked();

    void on_Registe_Btn_clicked();

    void receivedb(QSqlDatabase db);

private:
    Ui::Register *ui;
    QSqlDatabase database;
    bool status;
    int port;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QHostAddress *serverIP;

    bool tableFlag;
    int max_id;
};

#endif // REGISTER_H
