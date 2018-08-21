#include "loginin.h"
#include "widget.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    LoginIn w;
    Widget wi;
    QObject::connect(&w,SIGNAL(firstpageshow(QString)),&wi,SLOT(receivesignal(QString)));
    w.show();

    return a.exec();
}
