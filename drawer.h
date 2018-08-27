/*
 * 登陆进入IM后的主界面Drawer。
 */

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
    void userInfoPage();                                //转到用户个人信息界面的槽函数
    void changetitle(QString s);                        //改变界面标题的槽函数
    void LoginOut();                                    //登出的槽函数
    void groupPage();
    void showChatWidget1();                             //显示聊天界面的槽函数
    void showChatWidget2();
    void showChatWidget3();
    void showChatWidget4();
    void showChatWidget5();
    void showChatWidget6();
private:
    QToolButton *toolBtn1;                               //预先设定的UDP群聊的好友按钮
    QToolButton *toolBtn2;
    QToolButton *toolBtn3;
    QToolButton *toolBtn4;
    QToolButton *toolBtn5;
    QToolButton *toolBtn6;

    QPushButton *user_Info_btn;                           //转到用户信息界面的按钮
    QPushButton *backtoLogin;                             //回到登陆界面的按钮
    Widget *chatWidget1;
    Widget *chatWidget2;
    Widget *chatWidget3;
    Widget *chatWidget4;
    Widget *chatWidget5;
    Widget *chatWidget6;

    //
    QToolButton *toolBtn7;                                 //转到TCP群聊的按钮
};

#endif // DRAWER_H
