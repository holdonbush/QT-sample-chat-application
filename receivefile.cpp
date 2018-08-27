#include "receivefile.h"
#include "ui_receivefile.h"
#include <iostream>
#include <QDataStream>
#include <QFile>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

const quint16 PORT = 3333;
const int DATA_STREAM_VERSION = QDataStream::Qt_5_8;

ReceiveFile::ReceiveFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReceiveFile)
{
    ui->setupUi(this);
    /* 进度条调零 */
    ui->progressBar->setValue(0);
    /* 启用监听按钮 */
    ui->pushButton->setEnabled(true);
    fileBytes = gotBytes = nameSize = 0;
    file = Q_NULLPTR;
    receive = Q_NULLPTR;
    server = new QTcpServer(this);
    /* 连接请求 -> 接受连接 */
    connect(server, SIGNAL(newConnection()),
            this, SLOT(accept_connect()));

}

ReceiveFile::~ReceiveFile()
{
    delete ui;
    delete server;
}

/*
 * 函数名：on_pushButton_clicked
 * 功能：监听该端口，实现文件接收
 * 参数：NULL
 * 返回值：void
 */
void ReceiveFile::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    if(!server->listen(QHostAddress::Any, PORT))
    {
        std::cerr << "*** Listen to Port Failed ***" << std::endl;
        qDebug() << server->errorString();
        close();
        ui->pushButton->setEnabled(false);
        return;
    }
    ui->label->setText(QString("Listing to Port %1").arg(PORT));

}

/*
 * 函数名：accept_connect
 * 功能：建立连接
 * 参数：NULL
 * 返回值：void
 */
void ReceiveFile::accept_connect()
{
    receive = server->nextPendingConnection();
    /* 有数据到 -> 接受数据 */
    connect(receive, SIGNAL(readyRead()),
            this, SLOT(recevie_file()));
    /* socket出错 -> 出错处理 */
    connect(receive, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(show_error(QAbstractSocket::SocketError)));
    ui->label->setText(QString("Connection Established!"));
    gotBytes = 0;
    server->close();
}

/*
 * 函数名：recevie_file()
 * 功能：接收文件
 * 参数：NULL
 * 返回值：void
 */
void ReceiveFile::recevie_file()
{
    QDataStream in(receive);
    in.setVersion(DATA_STREAM_VERSION);

    /* 首部未接收/未接收完 */
    if(gotBytes <= 2 * sizeof(qint64))
    {
        if(!nameSize) // 前两个长度字段未接收
        {
            if(receive->bytesAvailable() >= 2 * sizeof(qint64))
            {
                in >> fileBytes >> nameSize;
                gotBytes += 2 * sizeof(qint64);
                ui->progressBar->setMaximum(fileBytes);
                ui->progressBar->setValue(gotBytes);
            }
            else // 数据不足，等下次
               return;
        }
        else if(receive->bytesAvailable() >= nameSize)
        {
            in >> fileName;
            gotBytes += nameSize;
            ui->progressBar->setValue(gotBytes);
            std::cout << "--- File Name: "
                      << fileName.toStdString() << std::endl;
        }
        else // 数据不足文件名长度，等下次
            return;
    }

    /* 已读文件名、文件未打开 -> 尝试打开文件 */
    if(!fileName.isEmpty() && file == Q_NULLPTR)
    {
        file = new QFile(fileName);
        if(!file->open(QFile::WriteOnly)) // 打开失败
        {
            std::cerr << "*** File Open Failed ***" << std::endl;
            delete file;
            file = Q_NULLPTR;
            return;
        }
        ui->label->setText(QString("Open %1 Successfully!").arg(fileName));
    }
    if(file == Q_NULLPTR) // 文件未打开，不能进行后续操作
        return;

    if(gotBytes < fileBytes) // 文件未接收完
    {
        gotBytes += receive->bytesAvailable();
        ui->progressBar->setValue(gotBytes);
        file->write(receive->readAll());
    }
    if(gotBytes == fileBytes) // 文件接收完
    {
        receive->close(); // 关socket
        file->close(); // 关文件
        delete file;
        ui->label->setText(QString("Finish receiving %1").arg(fileName));
        ui->pushButton->setEnabled(true);
    }
}

/*
 * 函数名：show_error
 * 功能：错误处理
 * 参数：NULL
 * 返回值：void
 */
void ReceiveFile::show_error(QAbstractSocket::SocketError)
{
    std::cerr << "*** Socket Error ***" << std::endl;
    qDebug() << receive->errorString();
    receive->close(); // 关cocket
    receive = Q_NULLPTR;
    file = Q_NULLPTR;
    fileName.clear(); // 清空文件名
    fileBytes = gotBytes = nameSize = 0;
    ui->progressBar->reset(); // 进度条归零
    ui->pushButton->setEnabled(true); // 启用监听按钮
    ui->label->setText(QString("*** SOCKET ERROR"));
}
