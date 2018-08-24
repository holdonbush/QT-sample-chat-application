#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class userInfo;
}

class userInfo : public QDialog
{
    Q_OBJECT

public:
    explicit userInfo(QWidget *parent = 0);
    ~userInfo();

private slots:
    void on_ChangeName_Btn_clicked();

    void on_ChangePwd_Btn_clicked();

    //void receivedb(QSqlDatabase db);

    void on_pushButton_clicked();

private:
    Ui::userInfo *ui;

    QSqlDatabase database;

};

#endif // USERINFO_H
