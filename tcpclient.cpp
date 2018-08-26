#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QCoreApplication>
#include <QFont>
#include <QFile>
#include <QFileDialog>


TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setWindowTitle("群聊");
    userName = windowTitle();

    status = false;
    port = 8010;
    ui->lineEdit_3->setText(QString::number(port));
    ui->lineEdit_2->setText("127.0.0.1");
    ui->lineEdit->setText(parent->windowTitle());
    serverIP = new QHostAddress();

    connect(ui->join_Btn,SIGNAL(clicked(bool)),this,SLOT(slotEnter()));
    connect(ui->send_Btn,SIGNAL(clicked(bool)),this,SLOT(slotSend()));
    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(curFmtChanged(QTextCharFormat)));
    ui->send_Btn->setEnabled(false);

    ui->toolButton->setIcon(QPixmap(":/img/bold.png"));
    ui->toolButton_2->setIcon(QPixmap(":/img/italic.png"));
    ui->toolButton_3->setIcon(QPixmap(":/img/underline.png"));
    ui->toolButton_4->setIcon(QPixmap(":/img/color.png"));
    ui->toolButton_5->setIcon(QPixmap(":/img/send.png"));
    ui->toolButton_6->setIcon(QPixmap(":/img/save.png"));
    ui->toolButton_7->setIcon(QPixmap(":/img/clear.png"));
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
        qDebug()<<"disconnect";
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
    QString str = windowTitle();
    QString msg = userName + ":" + "+" + ui->textEdit->toHtml();
    ui->textBrowser->append(userName+":");
    //QByteArray str = msg.toUtf8();
    //str.append("\n");
    tcpSocket->write(msg.toLocal8Bit());
    ui->textEdit->clear();
    //ui->listWidget->addItem(msg.left(msg.size()));
}

void TcpClient::slotDisconnected()
{
    ui->send_Btn->setEnabled(false);
    ui->join_Btn->setText(tr("进入聊天室"));
}

void TcpClient::dataReceived()
{
   /* while(tcpSocket->bytesAvailable() > 0)
    {
        qDebug()<<"read";
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg = datagram.data();
        ui->listWidget->addItem(msg.left(datagram.size()).toUtf8());
    } */
    qDebug()<<"receive";
    QByteArray byte = tcpSocket->readAll();
    QString str = QString::fromLocal8Bit(byte);
    //ui->listWidget->addItem(str.left(str.size()));
    ui->textBrowser->append(str);
}

void TcpClient::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void TcpClient::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->textEdit->setFontPointSize(arg1.toDouble());
    ui->textEdit->setFocus();
}

void TcpClient::on_toolButton_clicked(bool checked)
{
    qDebug()<<checked;
    qDebug()<<"F";
    if(checked)
    {
        qDebug()<<"bold";
        ui->textEdit->setFontWeight(QFont::Bold);
    }
    else {
        qDebug()<<"normal";
        ui->textEdit->setFontWeight(QFont::Normal);
    }
    ui->textEdit->setFocus();
}

void TcpClient::on_toolButton_2_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

void TcpClient::on_toolButton_3_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

void TcpClient::on_toolButton_4_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

void TcpClient::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontComboBox->setCurrentFont(fmt.font());
    if(fmt.fontPointSize() < 8)
    {
        ui->comboBox->setCurrentIndex(4);
    }
    else {
        ui->comboBox->setCurrentIndex(ui->comboBox->findText(QString::number(fmt.fontPointSize())));
    }
    ui->toolButton->setChecked(fmt.font().bold());
    ui->toolButton_2->setChecked(fmt.font().italic());
    ui->toolButton_3->setChecked(fmt.font().underline());
    color = fmt.foreground().color();
}

void TcpClient::on_toolButton_6_clicked()
{
    if(ui->textBrowser->document()->isEmpty())
    {
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存"),QMessageBox::Ok);
    }
    else
    {
        QString fname = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),tr("文本(*.txt);;所有文件(*.*)"));
        if(!fname.isEmpty())
        {
            saveFile(fname);
        }
    }
}

bool TcpClient::saveFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,tr("保存文件"),tr("无法保存文件 %1:\n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->textBrowser->toPlainText();
    return true;
}
void TcpClient::on_toolButton_7_clicked()
{
    ui->textBrowser->clear();
}
