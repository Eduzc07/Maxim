#include "include/readdata.h"

#include <QDebug>
#include <QFile>
#include <QDateTime>

#include <QTimer>

ReadData::ReadData(QObject *parent) :
    QObject(parent)
{
    qDebug()<< "-------------------";
}

void ReadData::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

void ReadData::readFile()
{
    qDebug()<< "-------readFile------------";
    QFile file(":/lista.csv");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<< "-------empty------------";
        return;
    }

//    m_qStart = QTime(22,35,0);

    m_qStart = QTime().currentTime();
    m_qStart.setHMS(m_qStart.hour(), m_qStart.minute() + 1, 0);
    qDebug()<< "====================tt-->" << m_qStart.toString("hh:mm:ss");
    QTime nextTime = m_qStart.addSecs(0);

    m_StartTime = nextTime.toString("hh:mm:ss");
    qDebug()<< "====================tt" << m_StartTime;



    QTextStream xin(&file);
    while(!xin.atEnd()){
        auto line = xin.readLine();
        auto values = line.split(",");
        const int colCount = values.size();

        qDebug()<<"---------"<< values;
        //Jump the first line
        int fRow = QString::compare(values[0], "Lugar", Qt::CaseInsensitive);  // if strings are equal x should return 0
        int emptyRow = QString::compare(values[0], "", Qt::CaseInsensitive);  // if strings are equal x should return 0
        if (fRow == 0)
            continue;

        for(int jx = 0; jx < colCount;jx++){
            saveData(jx, values.at(jx));
            qDebug()<< xin.pos() << "-------------------"<< values.at(jx);
        }

        //Add new rider
        m_posStart++;

        emit riderChanged();
        qDebug()<< "====================";
    }
    file.close();

}

void ReadData::saveData(int id, QString value)
{

    switch (id) {
    case 0:
        m_position = value;
        break;
    case 1:
        m_name = value;
        break;
    case 2:
        m_cat = value;
        break;
    case 3:
        m_club = value;
        break;
    case 4:
        m_home = value;
        break;
    case 5:
        m_num = value;

        int emptyRow = QString::compare(m_num, "", Qt::CaseInsensitive);  // if strings are equal x should return 0
        if (emptyRow == 0){
            m_StartTime = "";
        }else{
            QTime nextTime = m_qStart.addSecs(m_posStart*m_interval);

            m_StartTime = nextTime.toString("hh:mm:ss");
//            changeNameTime(m_StartTime);
            qDebug()<< "====================tt" << m_StartTime;
        }
        break;
    }
}

void ReadData::changeNameTime(QString& time)
{
    auto values = time.split(":");
    QString hh = values[0];
    QString mm = values[1];
    QString ss = values[2];
    QString newTime = QString("%1h%2m%3s")
            .arg(hh).arg(mm).arg(ss);
    time = newTime;
}

