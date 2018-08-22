#include "loginin.h"
#include "widget.h"
#include "drawer.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    LoginIn w;
    Drawer d;
    d.resize(250,700);
    QObject::connect(&w,SIGNAL(firstpageshow(QString)),&d,SLOT(receivesignal(QString)));
    w.show();

    return a.exec();
}
