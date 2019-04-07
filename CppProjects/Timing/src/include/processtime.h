#ifndef PROCESSTIME_H
#define PROCESSTIME_H

#include <QObject>
#include <QString>
#include <QTime>

class ProcessTime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString elapsed READ elapsed)

public:
    explicit ProcessTime(QObject *parent = nullptr);

    QString elapsed() const { return m_elapsed;}

    Q_INVOKABLE void setTimeRef(QString);

    Q_INVOKABLE void getPos(QString);
    Q_INVOKABLE QString getTime(QString, QString);
    Q_INVOKABLE QString getRunTime(QString, QString);
    Q_INVOKABLE int getDiffSeg(QString, QString);

private:
    QString m_elapsed;
    QTime m_firstTime;
};

#endif // PROCESSTIME_H
