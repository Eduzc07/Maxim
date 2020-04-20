#include "superitem.h"
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtGui>

#include <QDebug>


SuperItem::SuperItem(QGraphicsItem * parent):
    QGraphicsItem(parent)
{
//    setFlag(QGraphicsItem::ItemIsMovable);
    Pressed =false;
    qDebug() << "Graphic Item";

}

QRectF SuperItem::boundingRect() const
{
    return QRectF(0,0,150,150);
}

void SuperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   QRectF rec = boundingRect();
   QBrush brush(Qt::blue);

   if (Pressed)
   {
       brush.setColor(Qt::red);

   }else{
       brush.setColor(Qt::green);
   }

   painter->fillRect(rec,brush);
   painter->drawRect(rec);
}


void SuperItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    qDebug() << "Press";
    qDebug() << event->pos();
    update();
    QGraphicsItem::mousePressEvent(event);
}

void SuperItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    qDebug() << "Release";
    qDebug() << event->pos();
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

