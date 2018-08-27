#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QCoreApplication>
#include <QFont>
#include <QFile>
#include <QFileDialog>
#include "sendfile.h"
#include "receivefile.h"


TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    setWindowTitle("群聊");
    userName = parent->windowTitle();

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

/*
 * 函数名：slotEnter
 * 功能：新用户连接
 * 参数：NULL
 * 返回值：void
 */
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

/*
 * 函数名：slotConnected
 * 功能：新用户进入聊天室
 * 参数：NULL
 * 返回值：void
 */
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

/*
 * 函数名：slotSend
 * 功能：发送消息
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::slotSend()
{
    if(ui->textEdit->toPlainText() == "")
    {
        return;
    }
    QString str = windowTitle();
    QString msg = userName + ":" + "+" + ui->textEdit->toHtml();
    ui->textBrowser->append(userName+":");
    tcpSocket->write(msg.toLocal8Bit());
    ui->textEdit->clear();
}

/*
 * 函数名：slotDisconnected
 * 功能：断开连接
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::slotDisconnected()
{
    ui->send_Btn->setEnabled(false);
    ui->join_Btn->setText(tr("进入聊天室"));
}

/*
 * 函数名：dataReceived
 * 功能：接收从服务端传来的数据
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::dataReceived()
{
    qDebug()<<"receive";
    QByteArray byte = tcpSocket->readAll();
    QString str = QString::fromLocal8Bit(byte);
    //ui->listWidget->addItem(str.left(str.size()));
    if(str == userName)
    {
        QMessageBox::StandardButton ok;
        ok = QMessageBox::information(this,tr("information"),tr("是否接收文件"),QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::Yes)
        {
            ReceiveFile *t = new ReceiveFile(this);
            t->show();
        }
    }
    else
    {
        ui->textBrowser->append(str);
    }
}

/*
 * 函数名：on_fontComboBox_currentFontChanged
 * 功能：改变字体
 * 参数：QFont
 * 返回值：void
 */
void TcpClient::on_fontComboBox_currentFontChanged(const QFont &f)
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

/*
 * 函数名：on_comboBox_currentIndexChanged
 * 功能：改变字体大小
 * 参数：QString
 * 返回值：void
 */
void TcpClient::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->textEdit->setFontPointSize(arg1.toDouble());
    ui->textEdit->setFocus();
}

/*
 * 函数名：on_toolButton_clicked
 * 功能：字体加粗
 * 参数：bool
 * 返回值：void
 */
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

/*
 * 函数名：on_toolButton_2_clicked
 * 功能：字体倾斜
 * 参数：bool
 * 返回值：void
 */
void TcpClient::on_toolButton_2_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

/*
 * 函数名：on_toolButton_3_clicked
 * 功能：字体加下划线
 * 参数：bool
 * 返回值：void
 */
void TcpClient::on_toolButton_3_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

/*
 * 函数名：on_toolButton_4_clicked
 * 功能：改变字体颜色
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::on_toolButton_4_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

/*
 * 函数名：curFmtChanged
 * 功能：适应在输入框中的不同字体
 * 参数：QTextCharFormat
 * 返回值：void
 */
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

/*
 * 函数名：on_toolButton_6_clicked
 * 功能：保存聊天记录
 * 参数：NULL
 * 返回值：void
 */
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

/*
 * 函数名：saveFile
 * 功能：保存文件
 * 参数：QString
 * 返回值：void
 */
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

/*
 * 函数名：on_toolButton_7_clicked
 * 功能：清空聊天记录
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::on_toolButton_7_clicked()
{
    ui->textBrowser->clear();
}

/*
 * 函数名：on_toolButton_5_clicked
 * 功能：发送文件
 * 参数：NULL
 * 返回值：void
 */
void TcpClient::on_toolButton_5_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("请选择对象"),QMessageBox::Ok);
        return;
    } 
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    int count = items.count();
    for(int i = 0; i < count; i++)
    {
        int row = ui->tableWidget->row(items.at(i));
        QTableWidgetItem *item = items.at(i);
        QString name = item->text(); //获取内容
        qDebug()<<name;
        QString str = "file+" + name;
        qDebug()<<str;
        tcpSocket->write(str.toLocal8Bit());
        tcpSocket->flush();
    }
    SendFile *s = new SendFile(this);
    s->show();
}

void TcpClient::on_toolButton_8_clicked()
{
    ReceiveFile *r = new ReceiveFile(this);
    r->show();
}
