#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsTextItem>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QCategoryAxis>

QT_CHARTS_USE_NAMESPACE

#include <QtCore/QDebug>

#include <QDebug>
#include <QMessageBox>
#include <QPointF>

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtGui/QMouseEvent>
#include <QtGui>

#include "example.h"
#include "superitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsItem *item;
    QGraphicsScene *scene;
    SuperItem *val;
    QPixmap image2;
    int num;

protected:

private slots:
    void on_action_Help_triggered();
    void on_pushButton_clicked();
    void on_action_Exit_triggered();
    void Receive_point(QPoint pi,QPoint pf);
};

#endif // MAINWINDOW_H
