#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "include/backend.h"
#include "include/readdata.h"
#include "include/processtime.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");
    qmlRegisterType<ReadData>("io.qt.readdata", 1, 0, "ReadData");
    qmlRegisterType<ProcessTime>("io.qt.processtime", 1, 0, "ProcessTime");

    // Using QQuickView
    QQuickView view;
    view.setSource(QUrl("qrc:/mainMenu.qml"));
    view.show();

    return app.exec();
}
