#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T17:11:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_OpenCV
TEMPLATE = app

#===================================================
#OpenCV
#===================================================
INCLUDEPATH += /usr/local/include/
LIBS += `pkg-config --libs opencv`


SOURCES += main.cpp\
        qcvwidget.cpp\
        opencvworker.cpp

HEADERS  += qcvwidget.h\
           opencvworker.h

FORMS    += qcvwidget.ui
