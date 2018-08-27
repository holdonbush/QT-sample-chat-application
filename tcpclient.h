/*
 * TCP群聊的用户客户端，与服务端信息交互，实现群聊功能
 */

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QColorDialog>
#include <QTextCharFormat>

namespace Ui {
class TcpClient;
}

class TcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();

private slots:
    void slotEnter();                //新用户进入
    void slotConnected();            //新连接成功
    void slotDisconnected();         //未连接
    void dataReceived();             //收到数据
    void slotSend();                 //发送内容框中的内容

    void on_fontComboBox_currentFontChanged(const QFont &f);   //改变字体样式

    void on_comboBox_currentIndexChanged(const QString &arg1);  //改变字体大小

    void on_toolButton_clicked(bool checked);        //字体加粗

    void on_toolButton_2_clicked(bool checked);      //字体倾斜

    void on_toolButton_3_clicked(bool checked);      //改变字体颜色

    void on_toolButton_4_clicked();                 //添加下划线

    void curFmtChanged(const QTextCharFormat &fmt);  //适应内容框的不用位置的字体

    void on_toolButton_6_clicked();              //保存聊天记录

    void on_toolButton_7_clicked();              //清空聊天记录

    void on_toolButton_5_clicked();              //发送文件

    void on_toolButton_8_clicked();

protected:
    bool saveFile(const QString &filename);

private:
    Ui::TcpClient *ui;
    bool status;
    int port;
    QHostAddress *serverIP;
    QString userName;
    QColor color;
    QTcpSocket *tcpSocket;
};

#endif // TCPCLIENT_H
