#-------------------------------------------------
#
# Project created by QtCreator 2018-08-21T14:32:59
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompanyIM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        loginin.cpp \
    register.cpp \
    widget.cpp \
    drawer.cpp \
    userinfo.cpp \
    tcpclient.cpp \
    server.cpp \
    client.cpp \
    sendfile.cpp \
    receivefile.cpp

HEADERS += \
        loginin.h \
    register.h \
    widget.h \
    drawer.h \
    userinfo.h \
    tcpclient.h \
    server.h \
    client.h \
    sendfile.h \
    receivefile.h

FORMS += \
        loginin.ui \
    register.ui \
    widget.ui \
    userinfo.ui \
    tcpclient.ui \
    server.ui \
    client.ui \
    sendfile.ui \
    receivefile.ui

RESOURCES += \
    companyimsource.qrc
