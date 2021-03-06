//UDP 群聊里文件传输客户端

#include "client.h"
#include "ui_client.h"
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    setWindowTitle("接受");

    //建立连接
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tClnt = new QTcpSocket(this);
    tPort = 5555;
    connect(tClnt,SIGNAL(readyRead()),this,SLOT(readMsg()));
    connect(tClnt,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(diaplayErr(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete ui;
}

/*
 * 函数名：diaplayErr
 * 功能：错误处理
 * 参数：QAbstractSocket::SocketError
 * 返回值：void
 */
void Client::diaplayErr(QAbstractSocket::SocketError sockErr)
{
    switch (sockErr) {
    case QAbstractSocket::RemoteHostClosedError:

        break;
    default:
        qDebug()<<tClnt->errorString();
        //break;
    }
}

/*
 * 函数名：newConn
 * 功能：建立新的连接到服务端，连接好之后开始计时
 * 参数：NULL
 * 返回值：void
 */
void Client::newConn()
{
    blockSize = 0;
    tClnt->abort();
    tClnt->connectToHost(hostAddr,tPort);
    time.start();   //计时器启动
}

/*
 * 函数名：readMsg
 * 功能：接收文件
 * 参数：NULL
 * 返回值：void
 */
void Client::readMsg()
{
    QDataStream in(tClnt);
    in.setVersion(QDataStream::Qt_5_8);
    float useTime = time.elapsed();
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((tClnt->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((tClnt->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            in >> fileName;
            bytesReceived += fileNameSize;
            if(!locFile->open(QFile::WriteOnly))
            {
                QMessageBox::warning(this,tr("应用程序"),tr("无法获取文件 %1：\n%2.").arg(fileName).arg(locFile->errorString()));
                return;
            }
        }
        else
        {
            return;
        }
    }
    if(bytesReceived < totalBytes)
    {
        bytesReceived += tClnt->bytesAvailable();
        inBlock = tClnt->readAll();
        locFile->write(inBlock);
        inBlock.resize(0);
    }
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesReceived);
    double speed = bytesReceived / useTime;
    ui->cStatusLbl->setText(tr("已接收 %1MB （%2MB/s） \n 共%3MB  已用时：%4秒 估计剩余时间：%5秒").arg(bytesReceived / (1024*1024))
                            .arg(speed*1000/(1024*1024),0,'f',2)
                            .arg(totalBytes / (1024*1024))
                            .arg(useTime / 1000 , 0,'f',0)
                            .arg(totalBytes/speed/1000 - useTime/1000,0,'f',0));
    if(bytesReceived == totalBytes)
    {
        locFile->close();
        tClnt->close();
        ui->cStatusLbl->setText(tr("接收文件 %1 完毕").arg(fileName));
    }
}

/*
 * 函数名：on_cCancleBtn_clicked
 * 功能：取消接受文件
 * 参数：NULL
 * 返回值：void
 */
void Client::on_cCancleBtn_clicked()
{
    tClnt->abort();
    if(locFile->isOpen())
    {
        locFile->close();
    }
}

/*
 * 函数名： on_cCloseBtn_clicked
 * 功能：关闭文件接收客户端
 * 参数：NULL
 * 返回值：void
 */
void Client::on_cCloseBtn_clicked()
{
    tClnt->abort();
    if(locFile->isOpen())
        locFile->close();
    close();
}

/*
 * 函数名：closeEvent
 * 功能：窗口关闭按钮的事件，绑定到客户端关闭按钮
 * 参数：NULL
 * 返回值：void
 */
void Client::closeEvent(QCloseEvent *)
{
    on_cCloseBtn_clicked();
}
