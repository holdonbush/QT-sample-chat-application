#include "widget.h"
#include "ui_widget.h"
#include "userinfo.h"
#include "server.h"
#include "client.h"
#include <QFileDialog>
#include <QColorDialog>

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
    connect(ui->msgTxtEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(curFmtChange(QTextCharFormat)));

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

/*
 * 函数名：sendMsg
 * 功能：发送消息
 * 返回值：void
 */
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

/*
 * 函数名：processPendingDatagram
 * 功能：UDP群聊功能中接收UDP信息
 * 返回值：void
 */
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

/*
 * 函数名：usrEnter
 * 功能：新用户上线
 * 返回值：void
 */
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

/*
 * 函数名：usrLeft
 * 功能：用户离开
 * 返回值：void
 */
void Widget::usrLeft(QString usrname, QString time)
{
    int rowNum = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).first()->row();
    ui->usrTblWidget->removeRow(rowNum);
    ui->msgBrowser->setTextColor(Qt::gray);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->msgBrowser->append(tr("%1 于 %2 离开").arg(usrname).arg(time));
    ui->usrNumLbl->setText(tr("在线人数 %1").arg(ui->usrTblWidget->rowCount()));
}

/*
 * 函数名：getIP
 * 功能：获取IP地址
 * 返回值：QString
 */
QString Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

/*
 * 函数名：getUsr
 * 功能：获取用户名
 * 返回值：QString
 */
QString Widget::getUsr()
{
    return uName;
}

/*
 * 函数名：getMsg
 * 功能：获取用户发送的消息
 * 返回值：QString
 */
QString Widget::getMsg()
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}

/*
 * 函数名：on_sendBtn_clicked
 * 功能：发送消息
 * 返回值：void
 */
void Widget::on_sendBtn_clicked()
{
    sendMsg(Msg);
}

/*
 * 函数名：on_exitBtn_clicked
 * 功能：离开
 * 返回值：void
 */
void Widget::on_exitBtn_clicked()
{
    //sendMsg(UsrLeft);
    this->close();
}

void Widget::getFileName(QString name)
{
    fileName = name;
    sendMsg(FileName);
}

/*
 * 函数名：on_sendTBtn_clicked
 * 功能：发送文件
 * 返回值：void
 */
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

/*
 * 函数名：hasPendingFile
 * 功能：判断是否要接收文件
 * 返回值：void
 */
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

/*
 * 函数名：on_fontCbx_currentFontChanged
 * 功能：改变字体
 * 返回值：void
 */
void Widget::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTxtEdit->setCurrentFont(f);
    ui->msgTxtEdit->setFocus();
}

/*
 * 函数名：on_comboBox_currentIndexChanged
 * 功能：改变字体大小
 * 返回值：void
 */
void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->msgTxtEdit->setFontPointSize(arg1.toDouble());
    ui->msgTxtEdit->setFocus();
}

/*
 * 函数名：on_boldTBtn_clicked
 * 功能：字体加粗
 * 返回值：void
 */
void Widget::on_boldTBtn_clicked(bool checked)
{
    qDebug()<<checked;
    qDebug()<<"F";
    if(checked)
    {
        qDebug()<<"bold";
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    }
    else {
        qDebug()<<"normal";
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    }
    ui->msgTxtEdit->setFocus();
}

/*
 * 函数名：on_italicTBtn_clicked
 * 功能：字体倾斜
 * 返回值：void
 */
void Widget::on_italicTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
}

/*
 * 函数名：on_underlineTBtn_clicked
 * 功能：字体添加下划线
 * 返回值：void
 */
void Widget::on_underlineTBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
}

/*
 * 函数名：on_colorTBtn_clicked
 * 功能：改变字体颜色
 * 返回值：void
 */
void Widget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->msgTxtEdit->setTextColor(color);
        ui->msgTxtEdit->setFocus();
    }
}

/*
 * 函数名：curFmtChange
 * 功能：适应内容输入框中不同位置的不同字体
 * 返回值：void
 */
void Widget::curFmtChange(const QTextCharFormat &fmt)
{
    ui->fontCbx->setCurrentFont(fmt.font());

    if(fmt.fontPointSize() < 8)
    {
        ui->comboBox->setCurrentIndex(4);
    }
    else
    {
        ui->comboBox->setCurrentIndex(ui->comboBox->findText(QString::number(fmt.fontPointSize())));
    }
    ui->boldTBtn->setChecked(fmt.font().bold());
    ui->italicTBtn->setChecked(fmt.font().italic());
    ui->underlineTBtn->setChecked(fmt.font().underline());
    color = fmt.foreground().color();
}

/*
 * 函数名：on_saveTBtn_clicked
 * 功能：保存聊天记录
 * 返回值：void
 */
void Widget::on_saveTBtn_clicked()
{
    if(ui->msgBrowser->document()->isEmpty())
    {
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存"),QMessageBox::Ok);
    }
    else {
        QString fname = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),tr("文本(*.txt);;所有文件(*.*)"));
        if(!fname.isEmpty())
        {
            saveFile(fname);
        }
    }
}

bool Widget::saveFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1:\n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->msgBrowser->toPlainText();
    return true;
}

/*
 * 函数名：on_clearTBtn_clicked
 * 功能：清空聊天记录
 * 返回值：void
 */
void Widget::on_clearTBtn_clicked()
{
    ui->msgBrowser->clear();
}

void Widget::closeEvent(QCloseEvent *event)
{
    sendMsg(UsrLeft);
    QWidget::closeEvent(event);
}
