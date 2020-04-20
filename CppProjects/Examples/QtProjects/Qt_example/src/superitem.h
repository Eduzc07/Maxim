#ifndef SUPERITEM_H
#define SUPERITEM_H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

#include <QtWidgets/QGraphicsSceneMouseEvent>

class SuperItem : public QGraphicsItem
{

public:
    SuperItem(QGraphicsItem * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool Pressed;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};
#endif // SUPERITEM_H
