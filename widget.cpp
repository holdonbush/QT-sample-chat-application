#include "widget.h"
#include "ui_widget.h"
#include "userinfo.h"
#include "server.h"
#include "client.h"
#include <QFileDialog>

Widget::Widget(QWidget *parent,QString usrname) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    uName = usrname;
    udpSocket = new QUdpSocket(this);
    port = 23232;
    udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
    sendMsg(UsrEnter);

    srv = new Server(this);
    connect(srv,SIGNAL(sendFileName(QString)),this,SLOT(getFileName(QString)));

    ui->boldTBtn->setIcon(QPixmap(":img/bold.png"));
    ui->italicTBtn->setIcon(QPixmap(":img/italic.png"));
    ui->underlineTBtn->setIcon(QPixmap(":img/underline.png"));
    ui->colorTBtn->setIcon(QPixmap(":img/color.png"));
    ui->sendTBtn->setIcon(QPixmap(":img/send.png"));
    ui->saveTBtn->setIcon(QPixmap(":img/save.png"));
    ui->clearTBtn->setIcon(QPixmap(":img/clear.png"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sendMsg(MsgType type, QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString address = getIP();
    out << type <<getUsr();
    switch (type) {
    case Msg:
        if(ui->msgTxtEdit->toPlainText() == "")
        {
            QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
            return;
        }
        out << address << getMsg();
        ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    case UsrEnter:
        out << address;
        break;
    case UsrLeft:
        break;
    case FileName:
    {
        int row = ui->usrTblWidget->currentRow();
        QString clntaddr = ui->usrTblWidget->item(row , 1)->text();
        out<<address<<clntaddr<<fileName;
        break;
    }
    case Refuse:

        out<<srvaddr;
        break;

    //default:
    //    break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void Widget::processPendingDatagram()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;
        QString usrName,ipAddr,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch (msgType) {
        case Msg:
            in >> usrName >> ipAddr >>msg;
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[ " +usrName +" ]" + time);
            ui->msgBrowser->append(msg);
            break;
        case UsrEnter:
            in >> usrName >> ipAddr;
            usrEnter(usrName,ipAddr);
            break;
        case UsrLeft:
            in >> usrName;
            usrLeft(usrName,time);
            break;
        case FileName:
        {
            in >> usrName >> ipAddr;
            QString clntAddr,fileName;
            in >> clntAddr >> fileName;
            hasPendingFile(usrName,ipAddr,clntAddr,fileName);
            break;
        }
        case Refuse:
        {
            in >> usrName;
            QString srvAddr;
            in >> srvAddr;
            QString ipAddr = getIP();
            if(ipAddr == srvAddr)
            {
                srv->refused();
            }
            break;
        }
        //default:
        //    break;
        }
    }
}

void Widget::usrEnter(QString usrname, QString ipaddr)
{
    bool isEmpty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *usr = new QTableWidgetItem(usrname);
        QTableWidgetItem *ip = new QTableWidgetItem(ipaddr);
        ui->usrTblWidget->insertRow(0);
        ui->usrTblWidget->setItem(0,0,usr);
        ui->usrTblWidget->setItem(0,1,ip);
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->msgBrowser->append(tr("%1 在线！").arg(usrname));
        ui->usrNumLbl->setText(tr("在线人数 %1").arg(ui->usrTblWidget->rowCount()));
        sendMsg(UsrEnter);
    }
}

void Widget::usrLeft(QString usrname, QString time)
{
    int rowNum = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).first()->row();
    ui->usrTblWidget->removeRow(rowNum);
    ui->msgBrowser->setTextColor(Qt::gray);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->msgBrowser->append(tr("%1 于 %2 离开").arg(usrname).arg(time));
    ui->usrNumLbl->setText(tr("在线人数 %1").arg(ui->usrTblWidget->rowCount()));
}

QString Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

QString Widget::getUsr()
{
    return uName;
}

QString Widget::getMsg()
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}
void Widget::on_sendBtn_clicked()
{
    sendMsg(Msg);
}

void Widget::on_exitBtn_clicked()
{
    sendMsg(UsrLeft);
    this->close();
}

void Widget::getFileName(QString name)
{
    fileName = name;
    sendMsg(FileName);
}


void Widget::on_sendTBtn_clicked()
{
    if(ui->usrTblWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0,tr("选择用户"),tr("请先选择目标用户！"),QMessageBox::Ok);
        return;
    }
    srv->show();
    srv->initSrv();
}

void Widget::hasPendingFile(QString usrname, QString srvaddr, QString clntaddr, QString filename)
{
    QString ipAddr = getIP();
    if(ipAddr == clntaddr)
    {
        int btn = QMessageBox::information(this,tr("接收文件"),tr("来自%1（%2）的文件：%3，是否接收").arg(usrname).arg(srvaddr).arg(filename),QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),filename);
            if(!name.isEmpty())
            {
                Client *clnt = new Client(this);
                clnt->setFileName(name);
                clnt->setHostAddr(QHostAddress(srvaddr));
                clnt->show();
            }
        }else {
            sendMsg(Refuse, srvaddr);
        }
    }
}

void Client::setFileName(QString name)
{
    locFile = new QFile(name);
}

void Client::setHostAddr(QHostAddress addr)
{
    hostAddr = addr;
    newConn();
}
