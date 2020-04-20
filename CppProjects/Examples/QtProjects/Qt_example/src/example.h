#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>

class example : public QGraphicsView
{
    Q_OBJECT

public:
    example(QWidget *parent = 0);
    QPoint point1;
    QPoint point2;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);


signals:

signals:
    void valueChanged(QPoint pi,QPoint pf);

private:
      QGraphicsRectItem *rect;
      QPen GreenPen;
      QPoint PointX,PointY;
      bool control=false;

      int m_value;

      void RectPos(QPoint,QPoint,QPoint&,QPoint&);


};

#endif // EXAMPLE_H
