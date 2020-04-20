#ifndef CURSOR_H
#define CURSOR_H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

class Cursor : public QGraphicsItem
{

public:
    Cursor(QGraphicsItem * parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:

public slots:
};

#endif // CURSOR_H
