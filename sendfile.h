#ifndef SENDFILE_H
#define SENDFILE_H

#include <QDialog>
#include <QAbstractSocket>
class QByteArray;
class QFile;
class QString;
class QTcpSocket;


namespace Ui {
class SendFile;
}

class SendFile : public QDialog
{
    Q_OBJECT

public:
    explicit SendFile(QWidget *parent = 0);
    ~SendFile();

private slots:
    void start_transfer();
    void continue_transfer(qint64);
    void show_error(QAbstractSocket::SocketError);


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SendFile *ui;
    QTcpSocket *send;
    QFile *file;
    QString fileName;
    /* 总数据大小，已发送数据大小，剩余数据大小，每次发送数据块大小 */
    qint64 fileBytes, sentBytes, restBytes, loadBytes;

};

#endif // SENDFILE_H
