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
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_toolButton_clicked(bool checked);

    void on_toolButton_2_clicked(bool checked);

    void on_toolButton_3_clicked(bool checked);

    void on_toolButton_4_clicked();

    void curFmtChanged(const QTextCharFormat &fmt);

    void on_toolButton_6_clicked();

    void on_toolButton_7_clicked();

    void on_toolButton_5_clicked();

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
