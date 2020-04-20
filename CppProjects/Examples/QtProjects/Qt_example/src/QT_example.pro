#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T15:27:00
#
#-------------------------------------------------

QT += charts
QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_example
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    example.cpp \
    superitem.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    example.h \
    superitem.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui


RESOURCES += \
    source.qrc
