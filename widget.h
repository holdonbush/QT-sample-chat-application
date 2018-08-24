#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostInfo>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QtNetwork/QNetworkInterface>
#include <QProcess>

class Server;
namespace Ui {
class Widget;
}
enum MsgType{Msg,UsrEnter,UsrLeft,FileName,Refuse};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString usrname);
    ~Widget();

protected:
    void usrEnter(QString usrname,QString ipaddr);
    void usrLeft(QString usrname,QString time);
    void sendMsg(MsgType type,QString srvaddr = "");
    QString getIP();
    QString getUsr();
    QString getMsg();
    void hasPendingFile(QString usrname,QString srvaddr,QString clntaddr,QString filename);

private slots:
    void processPendingDatagram();

    void on_sendBtn_clicked();

    void on_exitBtn_clicked();

    void getFileName(QString);


    void on_sendTBtn_clicked();

private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
    QString uName_1;
    QString fileName;
    Server *srv;
};

#endif // WIDGET_H
