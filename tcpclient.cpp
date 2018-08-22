#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setWindowTitle("群聊");

    status = false;
    port = 8010;
    ui->lineEdit_3->setText(QString::number(port));
    ui->lineEdit_2->setText("127.0.0.1");
    ui->lineEdit->setText(parent->windowTitle());
    serverIP = new QHostAddress();
    connect(ui->join_Btn,SIGNAL(clicked(bool)),this,SLOT(slotEnter()));
    connect(ui->send_Btn,SIGNAL(clicked(bool)),this,SLOT(slotSend()));
    ui->send_Btn->setEnabled(false);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::slotEnter()
{
    if(!status)
    {
        QString ip = ui->lineEdit_2->text();
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }
        if(ui->lineEdit->text() == "")
        {
            QMessageBox::information(this,tr("error"),tr("username can not be empty!"));
            return;
        }
        userName = ui->lineEdit->text();
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        tcpSocket->connectToHost(*serverIP , port);
        status = true;
    }
    else
    {
        int length = 0;
        QString msg = userName + tr(":Leave Chat Room");
        if((length = tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
        {
            return;
        }
        tcpSocket->disconnectFromHost();
        status = false;
    }
}

void TcpClient::slotConnected()
{
    ui->send_Btn->setEnabled(true);
    ui->join_Btn->setText(tr("离开"));
    int length = 0;
    QString msg = userName + tr(":Enter Chat Room");
    if((length = tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        return;
    }
}

void TcpClient::slotSend()
{
    if(ui->textEdit->toPlainText() == "")
    {
        return;
    }
    QString msg = userName + ":" + ui->textEdit->toPlainText();

    //QByteArray str = msg.toUtf8();
    tcpSocket->write(msg.toLatin1(),msg.length());
    ui->textEdit->clear();
}

void TcpClient::slotDisconnected()
{
    ui->send_Btn->setEnabled(false);
    ui->join_Btn->setText(tr("进入聊天室"));
}

void TcpClient::dataReceived()
{
    while(tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg = datagram.data();
        ui->listWidget->addItem(msg.left(datagram.size()));
    }
}
