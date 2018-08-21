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
    QObject::connect(&w,SIGNAL(firstpageshow()),&wi,SLOT(receivesignal()));
    w.show();

    return a.exec();
}
