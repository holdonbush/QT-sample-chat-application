#include "sendfile.h"
#include "ui_sendfile.h"
#include <QByteArray>
#include <QDataStream>
#include <QFileDialog>
#include <QHostAddress>
#include <QIODevice>
#include <QString>
#include <QTcpSocket>

const quint16 PORT = 3333;
const qint64 LOADBYTES = 4 * 1024; // 4 kilo-byte
const int DATA_STREAM_VERSION = QDataStream::Qt_5_8;


SendFile::SendFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendFile)
{
    ui->setupUi(this);
    send = new QTcpSocket(this);
    fileBytes = sentBytes = restBytes = 0;
    loadBytes = LOADBYTES;
    file = Q_NULLPTR;
    ui->progressBar->setValue(0); // 进度条置零
    ui->pushButton_2->setEnabled(false); // 禁用发送按钮
    ui->pushButton_3->setEnabled(false);
    /* 连接已建立 -> 开始发数据 */
    connect(send, SIGNAL(connected()),
            this, SLOT(start_transfer()));
    /* 数据已发出 -> 继续发 */
    connect(send, SIGNAL(bytesWritten(qint64)),
            this, SLOT(continue_transfer(qint64)));
    /* socket出错 -> 错误处理 */
    connect(send, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(show_error(QAbstractSocket::SocketError)));

}

SendFile::~SendFile()
{
    delete ui;
    delete send;
}

/*
 * 函数名：on_pushButton_clicked
 * 功能：选择文件
 * 参数：NULL
 * 返回值：void
 */
void SendFile::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        ui->label->setText(
            QString("File %1 Opened!").arg(fileName));
        ui->pushButton_2->setEnabled(true);
    }
    else
        ui->label->setText(QString("*** FAIL OPENING FILE"));

}

/*
 * 函数名：on_pushButton_2_clicked
 * 功能：发送文件
 * 参数：NULL
 * 返回值：void
 */
void SendFile::on_pushButton_2_clicked()
{
    send->connectToHost(QHostAddress::LocalHost, PORT);
    sentBytes = 0;
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    ui->label->setText(QString("Linking..."));

}

/*
 * 函数名：start_transfer
 * 功能：开始传输
 * 参数：NULL
 * 返回值：void
 */
void SendFile::start_transfer()
{
    file = new QFile(fileName);
    if(!file->open(QFile::ReadOnly))
    {
        ui->label->setText(QString("*** FILE OPEN ERROR"));
        qDebug() << "*** start_transfer(): File-Open-Error";
        return;
    }

    fileBytes = file->size();
    ui->progressBar->setValue(0);
    ui->label->setText(QString("Connection Established!"));

    QByteArray buf;
    QDataStream out(&buf, QIODevice::WriteOnly);
    out.setVersion(DATA_STREAM_VERSION);

    /* 无路径文件名 */
    QString sfName = fileName.right(fileName.size() -
            fileName.lastIndexOf('/') - 1);
    /* 首部 = 总大小 + 文件名长度 + 文件名 */
    out << qint64(0) << qint64(0) << sfName;
    /* 总大小加上首部的大小 */
    fileBytes += buf.size();
    ui->progressBar->setMaximum(fileBytes);
    /* 重写首部的前两个长度字段 */
    out.device()->seek(0);
    out << fileBytes << (qint64(buf.size()) - 2 * sizeof(qint64));
    /* 发送首部，计算剩余大小 */
    restBytes = fileBytes - send->write(buf);

}

/*
 * 函数名：continue_transfer
 * 功能：继续传输
 * 参数：qint64
 * 返回值：void
 */
void SendFile::continue_transfer(qint64 sentSize)
{
    sentBytes += sentSize;
    ui->progressBar->setValue(sentBytes);
    /* 还有数据要发 */
    if(restBytes > 0)
    {
        /* 从文件读数据 */
        QByteArray buf = file->read(qMin(loadBytes, restBytes));
        /* 发送 */
        restBytes -= send->write(buf);
    }
    else
        file->close();
    /* 全部发送完 */
    if(sentBytes == fileBytes)
    {
        send->close(); // 关socket
        fileName.clear(); // 清空文件名
        ui->label->setText(QString("Finish sending!"));
    }
}

/*
 * 函数名：show_error
 * 功能：错误处理
 * 参数：QAbstractSocket::SocketError
 * 返回值：void
 */
void SendFile::show_error(QAbstractSocket::SocketError)
{
    qDebug() << "*** Socket Error";
    send->close();
    ui->label->setText(QString("*** SOCKET ERROR, RESEND LATER"));
    ui->pushButton_2->setEnabled(true);
    ui->progressBar->reset(); // 进度条归零
    fileName.clear();
}
