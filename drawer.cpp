#include "drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>

Drawer::Drawer(QWidget *parent, Qt::WindowFlags f)
    :QToolBox(parent,f)
{
    setWindowTitle("Company IM");
    setWindowIcon(QPixmap(":/img/cat.png"));

    toolBtn1 = new QToolButton;
    toolBtn1->setText("1");
    toolBtn1->setIcon(QPixmap(":/img/cat.png"));
    toolBtn1->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn1->setAutoRaise(true);
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn1,SIGNAL(clicked(bool)),this,SLOT(showChatWidget1()));

    toolBtn2 = new QToolButton;
    toolBtn2->setText("2");
    toolBtn2->setIcon(QPixmap(":/img/cat.png"));
    toolBtn2->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn2->setAutoRaise(true);
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn2,SIGNAL(clicked(bool)),this,SLOT(showChatWidget2()));

    toolBtn3 = new QToolButton;
    toolBtn3->setText("3");
    toolBtn3->setIcon(QPixmap(":/img/cat.png"));
    toolBtn3->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn3->setAutoRaise(true);
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn3,SIGNAL(clicked(bool)),this,SLOT(showChatWidget3()));

    toolBtn4 = new QToolButton;
    toolBtn4->setText("4");
    toolBtn4->setIcon(QPixmap(":/img/cat.png"));
    toolBtn4->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn4->setAutoRaise(true);
    toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn4,SIGNAL(clicked(bool)),this,SLOT(showChatWidget4()));

    toolBtn5 = new QToolButton;
    toolBtn5->setText("5");
    toolBtn5->setIcon(QPixmap(":/img/cat.png"));
    toolBtn5->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn5->setAutoRaise(true);
    toolBtn5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn5,SIGNAL(clicked(bool)),this,SLOT(showChatWidget5()));

    toolBtn6 = new QToolButton;
    toolBtn6->setText("6");
    toolBtn6->setIcon(QPixmap(":/img/cat.png"));
    toolBtn6->setIconSize(QPixmap(":/img/cat.png").size());
    toolBtn6->setAutoRaise(true);
    toolBtn6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn6,SIGNAL(clicked(bool)),this,SLOT(showChatWidget6()));

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *Layout = new QVBoxLayout(groupBox);

    Layout->setMargin(20);
    Layout->setAlignment(Qt::AlignLeft);
    Layout->addWidget(toolBtn1);
    Layout->addWidget(toolBtn2);
    Layout->addWidget(toolBtn3);
    Layout->addWidget(toolBtn4);
    Layout->addWidget(toolBtn5);
    Layout->addWidget(toolBtn6);

    Layout->addStretch();
    this->addItem((QWidget*)groupBox,tr("群成员"));
}

void Drawer::receivesignal(QString name)
{
    this->show();
    this->setWindowTitle(name);
}

void Drawer::showChatWidget1()
{
    chatWidget1 = new Widget(0,toolBtn1->text());
    chatWidget1->setWindowTitle(toolBtn1->text());
    chatWidget1->setWindowIcon(toolBtn1->icon());
    chatWidget1->show();
}

void Drawer::showChatWidget2()
{
    chatWidget2 = new Widget(0,toolBtn2->text());
    chatWidget2->setWindowTitle(toolBtn2->text());
    chatWidget2->setWindowIcon(toolBtn2->icon());
    chatWidget2->show();
}

void Drawer::showChatWidget3()
{
    chatWidget3 = new Widget(0,toolBtn3->text());
    chatWidget3->setWindowTitle(toolBtn3->text());
    chatWidget3->setWindowIcon(toolBtn3->icon());
    chatWidget3->show();
}

void Drawer::showChatWidget4()
{
    chatWidget4 = new Widget(0,toolBtn4->text());
    chatWidget4->setWindowTitle(toolBtn4->text());
    chatWidget4->setWindowIcon(toolBtn4->icon());
    chatWidget4->show();
}

void Drawer::showChatWidget5()
{
    chatWidget5 = new Widget(0,toolBtn5->text());
    chatWidget5->setWindowTitle(toolBtn5->text());
    chatWidget5->setWindowIcon(toolBtn5->icon());
    chatWidget5->show();
}

void Drawer::showChatWidget6()
{
    chatWidget6 = new Widget(0,toolBtn6->text());
    chatWidget6->setWindowTitle(toolBtn6->text());
    chatWidget6->setWindowIcon(toolBtn6->icon());
    chatWidget6->show();
}
