#include "include/processtime.h"

#include <QDebug>
#include <QtMath>

ProcessTime::ProcessTime(QObject *parent) :
    QObject(parent)
{
    m_firstTime = QTime(0,0,0,0);
}

void ProcessTime::setTimeRef(QString time)
{
    m_firstTime = QTime::fromString("00:" + time);
}

void ProcessTime::getPos(QString time)
{
    int nsp = QString::compare(time, "N.S.P.", Qt::CaseInsensitive);
    if (nsp == 0){
        m_elapsed = "";
        return;
    }

    QTime currentTime = QTime::fromString("00:" + time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();

    //Get difference from the first
    millisecondsDiff = m_firstTime.msecsTo(currentTime);

    if( millisecondsDiff == 0){
        m_elapsed = "";
        return;
    }

    QString sign = "+";

    if (millisecondsDiff < 0){
        millisecondsDiff = currentTime.msecsTo(m_firstTime);
        sign = "-";
        m_firstTime = currentTime;
    }

    int diff_ms = static_cast<int>(millisecondsDiff);
    m_elapsed = sign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz");
}

QString ProcessTime::getTime(QString currentTime, QString startTime)
{
    QTime time1 = QTime::fromString(currentTime);
    QTime time0 = QTime::fromString(startTime);
    qint64 millisecondsDiff = time0.msecsTo(time1);
    int t_ms = static_cast<int>(millisecondsDiff);
    return QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss.zzz");
}

QString ProcessTime::getRunTime(QString currentTime, QString startTime)
{
    QTime time1 = QTime::fromString(currentTime);
    QTime time0 = QTime::fromString(startTime);
    qint64 millisecondsDiff = time0.msecsTo(time1);
    int t_ms = static_cast<int>(millisecondsDiff);
//    t_ms += 1000;
    return QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss");
}

int ProcessTime::getDiffSeg(QString currentTime, QString startTime)
{
    QTime time1 = QTime::fromString(currentTime);
    QTime time0 = QTime::fromString(startTime);
    qint64 millisecondsDiff = time1.msecsTo(time0);
    int t_ms = static_cast<int>(millisecondsDiff);
    double val = t_ms /1000.0;
    return qCeil(val);
}
