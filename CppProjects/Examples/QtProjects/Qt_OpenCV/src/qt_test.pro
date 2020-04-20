#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T19:46:33
#
#-------------------------------------------------
QT    += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_test
TEMPLATE = app
INCLUDEPATH += /usr/local/include/opencv4/opencv2
LIBS += `pkg-config --libs opencv`

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui
