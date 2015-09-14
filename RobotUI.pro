#-------------------------------------------------
#
# Project created by QtCreator 2013-12-15T22:10:08
#
#-------------------------------------------------

QT       += core gui
QT       += network #needs this for sockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpsocket.cpp \
    commandHandle.cpp \
    opencvHandle.cpp \
    cvideolabel.cpp

HEADERS  += mainwindow.h \
    tcpsocket.h \
    commandHandle.h \
    opencvHandle.h \
    cvideolabel.h

LIBS += `pkg-config --libs opencv` #needed for opencv

FORMS    += mainwindow.ui
