#include "example.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>

#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include <QDebug>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>

#include <QtCharts>

example::example(QWidget *parent)
    : QGraphicsView(parent)
{
//    setDragMode(QGraphicsView::NoDrag);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    GreenPen.setColor(Qt::green);
    GreenPen.setWidth(3);
    qDebug() << "Graphic";


//    this->setMouseTracking(true);
}

void example::mousePressEvent(QMouseEvent *event)
{
    if (control){
        scene()->removeItem(rect);
    }
    point1 = event->pos();//get global position according to ur parent-child relationship
//    double rad = 3;
    QPoint pt = event->pos();
//    scene()->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::blue));
    rect =  scene()->addRect(pt.x(),pt.y(),2,2,GreenPen);
    QGraphicsView::mouseMoveEvent(event);
}

void example::mouseReleaseEvent(QMouseEvent *event)
{
    point2 = event->pos();//get global position according to ur parent-child relationship
    control=true;
     RectPos(point1,point2,PointX,PointY);
    emit valueChanged(PointX,PointY);

}

void example::mouseMoveEvent(QMouseEvent *event)
{

//    double rad = 3;
//    QPoint pt = event->pos();
//    scene()->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0,QPen(), QBrush(Qt::blue));
//    QGraphicsView::mouseMoveEvent(event);

    scene()->removeItem(rect);
    point2 = event->pos();//get global position according to ur parent-child relationship

    RectPos(point1,point2,PointX,PointY);
    rect =  scene()->addRect(PointX.x(),PointX.y(),PointY.x(),PointY.y(),GreenPen);

//   QGraphicsView::mouseMoveEvent(event);
//   qDebug() << event->pos();

}

void example::RectPos(QPoint point1, QPoint point2,QPoint &pointA, QPoint &pointB){

    int w,h;
    QPoint P1,P2;

    if (point2.x()>=point1.x() && point2.y()>=point1.y())
    {
        h=point2.y()-point1.y();
        w=point2.x()-point1.x();
        P1=QPoint(point1.x(),point1.y());
        P2=QPoint(w,h);
    }

    if (point2.x()<point1.x() && point2.y()>point1.y())
    {
        h=point2.y()-point1.y();
        w=point1.x()-point2.x();
        P1=QPoint(point2.x(),point1.y());
        P2=QPoint(w,h);
    }

    if (point2.x()>point1.x() && point2.y()<point1.y())
    {
        h=point1.y()-point2.y();
        w=point2.x()-point1.x();
        P1=QPoint(point1.x(),point2.y());
        P2=QPoint(w,h);
    }

    if (point2.x()<point1.x() && point2.y()<point1.y())
    {
        h=point1.y()-point2.y();
        w=point1.x()-point2.x();
        P1=QPoint(point2.x(),point2.y());
        P2=QPoint(w,h);
    }

    pointA=P1;
    pointB=P2;
}

