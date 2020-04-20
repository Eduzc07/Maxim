#include "Cursor.h"
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>

#include <QDebug>

Cursor::Cursor(QGraphicsItem * parent):
    QGraphicsItem(parent)
{

//    qDebug() << "Graphic Item";

}

void Cursor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Cursor::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}
