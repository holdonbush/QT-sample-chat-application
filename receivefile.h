#ifndef RECEIVEFILE_H
#define RECEIVEFILE_H

#include <QDialog>
#include <QAbstractSocket>
class QFile;
class QString;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class ReceiveFile;
}

class ReceiveFile : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveFile(QWidget *parent = 0);
    ~ReceiveFile();

private slots:
    void accept_connect();
    void recevie_file();
    void show_error(QAbstractSocket::SocketError);


    void on_pushButton_clicked();

private:
    Ui::ReceiveFile *ui;
    QTcpServer *server;
    QTcpSocket *receive;
    QString fileName;
    QFile *file;
    /* 已接受数据，总数据，文件名长度 */
    qint64 gotBytes, fileBytes, nameSize;

};

#endif // RECEIVEFILE_H
