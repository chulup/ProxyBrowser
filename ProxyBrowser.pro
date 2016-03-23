#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T21:34:24
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = ProxyBrowser
TEMPLATE = app

CONFIG += c++11

SOURCES += sources/main.cpp\
        sources/mainwindow.cpp \
    sources/qblankwebpage.cpp \
    sources/filedownloader.cpp

HEADERS  += sources/mainwindow.h \
    sources/constants.h \
    sources/qblankwebpage.h \
    sources/filedownloader.h

FORMS    += forms/mainwindow.ui

RESOURCES += \
    icons.qrc

ICON = icone2.icns

DISTFILES += \
    config.ini
