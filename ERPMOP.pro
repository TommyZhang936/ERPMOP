#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T13:37:09
#
#-------------------------------------------------

QT       += core gui sql xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ERPMOP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    myapp.cpp \
    user.cpp \
    mainwork.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    myapp.h \
    user.h \
    mainwork.h

FORMS += \
    logindialog.ui \
    user.ui \
    mainwork.ui

RESOURCES += \
    erpmop.qrc

win32:RC_FILE=erpmop.rc

DISTFILES +=
