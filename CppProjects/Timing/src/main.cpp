#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QScreen>

#include "include/readdata.h"
#include "include/processtime.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/icon.png"));
    app.setOrganizationName("E2 Innovation");
    app.setOrganizationDomain("e2i.com.pe");
    app.setApplicationName("Tmining Desktop Application");

    qmlRegisterType<ReadData>("io.qt.readdata", 1, 0, "ReadData");
    qmlRegisterType<ProcessTime>("io.qt.processtime", 1, 0, "ProcessTime");

//    QScreen *screen = QGuiApplication::primaryScreen();
    QList<QScreen*> listScreen = QGuiApplication::screens();
//    for (int i = 0; i < listScreen.size(); ++i) {
//        qDebug()<<listScreen.at(i)->geometry();
//    }
    QRect screenGeometry = listScreen.at(0)->geometry();


    QQmlApplicationEngine appMainWindow;
    appMainWindow.load(QUrl("qrc:/main.qml"));
    if (appMainWindow.rootObjects().isEmpty())
        return -1;

    QQuickWindow* quickWindow = qobject_cast<QQuickWindow*>(appMainWindow.rootObjects()[0]);
    quickWindow->setMinimumSize(QSize(600, 480));
    quickWindow->setMaximumSize(screenGeometry.size());
    quickWindow->setTitle("Timing Program");
    quickWindow->setGeometry(screenGeometry);

    return app.exec();
}
