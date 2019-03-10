#include "include/timethread.h"

#include <QDebug>
#include <QTime>

TimeThread::TimeThread()
{
}

void TimeThread::run(){

    while(true){
        QString as = QTime::currentTime().toString("HH:mm:ss.sss");
        if (QString::compare(mLastTime, as, Qt::CaseInsensitive)==0)
            continue;

        qDebug()<< "===" <<as;
        wait(100);
        mLastTime = as;
    }
}
