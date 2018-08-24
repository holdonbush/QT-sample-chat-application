#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>

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

private:
    Ui::userInfo *ui;
};

#endif // USERINFO_H
