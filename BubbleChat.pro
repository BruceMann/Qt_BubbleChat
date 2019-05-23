#-------------------------------------------------
#
# Project created by QtCreator 2019-05-22T15:11:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BubbleChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qnchatmessage.cpp \
    bubble.cpp

HEADERS  += mainwindow.h \
    qnchatmessage.h \
    bubble.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
