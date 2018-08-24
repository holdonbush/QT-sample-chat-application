#ifndef DRAWER_H
#define DRAWER_H
#include <QToolBox>
#include <QToolButton>
#include <QPushButton>
#include "widget.h"

class Drawer : public QToolBox
{
    Q_OBJECT
public:
    Drawer(QWidget *parent = 0,Qt::WindowFlags f = 0);

signals:
    void showloginpage();

private slots:
    void receivesignal(QString name);
    void userInfoPage();
    void LoginOut();
    void groupPage();
    void showChatWidget1();
    void showChatWidget2();
    void showChatWidget3();
    void showChatWidget4();
    void showChatWidget5();
    void showChatWidget6();
private:
    QToolButton *toolBtn1;
    QToolButton *toolBtn2;
    QToolButton *toolBtn3;
    QToolButton *toolBtn4;
    QToolButton *toolBtn5;
    QToolButton *toolBtn6;

    QPushButton *user_Info_btn;
    QPushButton *backtoLogin;
    Widget *chatWidget1;
    Widget *chatWidget2;
    Widget *chatWidget3;
    Widget *chatWidget4;
    Widget *chatWidget5;
    Widget *chatWidget6;

    //
    QToolButton *toolBtn7;
};

#endif // DRAWER_H
