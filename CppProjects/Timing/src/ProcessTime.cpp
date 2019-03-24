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

    qint64 millisecondsDiff = m_firstTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);
    integerVector.append(t_ms);
}

int ProcessTime::getDiff(QString time)
{
    QTime currentTime = QTime::fromString("00:" + time);
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
    QString sign = "+";
    m_color = "red";

    if (millisecondsDiff < 0){
        millisecondsDiff = currentTime.msecsTo(m_firstTime);
        colorSign = "<font color=\"green\">-";
        sign = "-";
        m_color = "green";
        m_firstTime = currentTime;
    }

    int diff_ms = static_cast<int>(millisecondsDiff);
    m_elapsed = colorSign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz")+ "</font>";
    m_flatElapsed = sign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz");
    return pos;
}

int ProcessTime::getPos(QString time)
{
    int nsp = QString::compare(time, "N.S.P.", Qt::CaseInsensitive);
    if (nsp == 0){
        m_elapsed = "";
        m_flatElapsed = m_elapsed;
        return 999999;
    }

    QTime currentTime = QTime::fromString("00:" + time);

    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);

//    qSort(integerVector.begin(), integerVector.end(), qLess<int>());
    QVector<int>::iterator i =
            qBinaryFind(integerVector.begin(), integerVector.end(), t_ms);
    int pos = static_cast<int>(i - integerVector.begin());

    //Get difference from the first
    millisecondsDiff = m_firstTime.msecsTo(currentTime);
    QString sign = "+";
    m_color = "red";

    if (millisecondsDiff < 0){
        millisecondsDiff = currentTime.msecsTo(m_firstTime);
        sign = "-";
        m_color = "green";
        m_firstTime = currentTime;
    }

    int diff_ms = static_cast<int>(millisecondsDiff);
    m_elapsed = sign + QTime::fromMSecsSinceStartOfDay(diff_ms).toString("mm:ss.zzz");
    m_flatElapsed = m_elapsed;
    pos++;
    return pos;
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

void ProcessTime::cleanRanking()
{
    integerVector.clear();
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

void ProcessTime::getColor(QString time)
{
    QTime currentTime = QTime::fromString("00:" + time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);
    m_color = "red";

    if (t_ms < integerVector.at(0))
        m_color = "green";

    if (t_ms <= 0)
        m_color = "gray";
}

void ProcessTime::storageRider(QString result)
{
    qDebug()<< "==========result=========="<< result;
    auto values = result.split(",");
    int number = values[0].toInt(); //Save Numebr
    QString time = values[1];       //Save time
    QString cat = values[2];        //Save cat

    QTime currentTime = QTime::fromString("00:" + time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);


    //If it is a new Category, create it
    if (m_iCatName.size() == 0) {
        m_iCatName.append(cat);
        QVector<int> timeChrono = {t_ms, number};
        QVector<QVector<int>> cat;
        cat.append(timeChrono);
        m_CatData.append(cat);

        //First rider
        return;
    }

    int posCat = 9999;
    for (int i = 0; i < m_iCatName.size(); ++i) {
        if (m_iCatName.at(i) == cat){
            posCat = i;
            break;
        }
    }

    if (posCat == 9999){
        m_iCatName.append(cat);
        QVector<int> timeChrono = {t_ms, number};
        QVector<QVector<int>> cat;
        cat.append(timeChrono);
        m_CatData.append(cat);
        return;
    }

    QVector<QVector<int>> categoryTime = m_CatData.value(posCat);
    QVector<int> timeChrono = {t_ms, number};
    categoryTime.append(timeChrono);

    std::sort(categoryTime.begin(),categoryTime.end() ,[](const QVector<int>& left,const QVector<int>& right)->bool{
                if(left.empty() && right.empty())
                    return false;
                if(left.empty())
                    return true;
                if(right.empty())
                    return false;
                return left.first()<right.first();
            }
        );

    m_CatData.replace(posCat, categoryTime);

    for (int j = 0; j < m_CatData.size(); ++j) {
        qDebug()<< "==>" << m_iCatName.at(j);
        for (int i = 0; i < m_CatData.at(j).size(); ++i) {
            qDebug()<< "==>" << m_CatData.at(j).at(i);
        }
    }
}
