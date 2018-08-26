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
#include <QTextCharFormat>


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
    bool saveFile(const QString &filename);
    void closeEvent(QCloseEvent *event);
private slots:
    void processPendingDatagram();

    void on_sendBtn_clicked();

    void on_exitBtn_clicked();

    void getFileName(QString);


    void on_sendTBtn_clicked();

    void on_fontCbx_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_boldTBtn_clicked(bool checked);

    void on_italicTBtn_clicked(bool checked);

    void on_underlineTBtn_clicked(bool checked);

    void on_colorTBtn_clicked();

    //void on_boldTBtn_toggled(bool checked);
    void curFmtChange(const QTextCharFormat &fmt);
    void on_saveTBtn_clicked();

    void on_clearTBtn_clicked();

private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
    QString uName_1;
    QString fileName;
    QColor color;
    Server *srv;
};

#endif // WIDGET_H
