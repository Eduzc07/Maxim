#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>
#include <QObject>
#include <QString>

class TimeThread : public QThread
{
Q_OBJECT
public:
    TimeThread();

protected:
    void run();

private:
    QString mLastTime;
};

#endif // TIMETHREAD_H
