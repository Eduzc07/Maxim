#include "include/processtime.h"

#include <QDebug>


ProcessTime::ProcessTime(QObject *parent) :
    QObject(parent)
{
    m_firstTime = QTime(0,0,0,0);
}

void ProcessTime::setTimeRef(QString time)
{
    m_firstTime = QTime::fromString(time);

    qDebug()<< "--------setTimeRef-----------" <<  m_firstTime.toString("mm:ss.zzz");

    qint64 millisecondsDiff = m_firstTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);
    integerVector.append(t_ms);

}

int ProcessTime::getDiff(QString time)
{
    QTime currentTime = QTime::fromString(time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);
    integerVector.append(t_ms);

    qSort(integerVector.begin(), integerVector.end(), qLess<int>());
    QVector<int>::iterator i =
            qBinaryFind(integerVector.begin(), integerVector.end(), t_ms);

    int pos = static_cast<int>(i - integerVector.begin());

    //Get difference from the first
    millisecondsDiff = m_firstTime.msecsTo(currentTime);
    QString colorSign = "<font color=\"red\">+";

    if (millisecondsDiff < 0){
        millisecondsDiff = currentTime.msecsTo(m_firstTime);
        colorSign = "<font color=\"green\">-";
        m_firstTime = currentTime;
    }

    int diff_ms = static_cast<int>(millisecondsDiff);
    m_elapsed = colorSign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz")+ "</font>";
    return pos;
}

int ProcessTime::getPos(QString time)
{
    QTime currentTime = QTime::fromString(time);

    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);

//    qSort(integerVector.begin(), integerVector.end(), qLess<int>());
    QVector<int>::iterator i =
            qBinaryFind(integerVector.begin(), integerVector.end(), t_ms);
    int pos = static_cast<int>(i - integerVector.begin());

    //Get difference from the first
    millisecondsDiff = m_firstTime.msecsTo(currentTime);
    QString sign = "+";

    if (millisecondsDiff < 0){
        millisecondsDiff = currentTime.msecsTo(m_firstTime);
        sign = "-";
        m_firstTime = currentTime;
    }

    int diff_ms = static_cast<int>(millisecondsDiff);
    m_elapsed = sign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz");
    return pos;
}
