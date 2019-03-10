#ifndef PROCESSTIME_H
#define PROCESSTIME_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QTime>

class ProcessTime : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString elapsed READ elapsed NOTIFY elapsedChanged)

public:
    explicit ProcessTime(QObject *parent = nullptr);

    QString elapsed() const { return m_elapsed;}
    Q_INVOKABLE void setTimeRef(QString);
    Q_INVOKABLE int getDiff(QString);
    Q_INVOKABLE int getPos(QString);

signals:
    void elapsedChanged();

private:
    QString m_elapsed;
    QTime m_firstTime;
    QVector<int> integerVector;
};

#endif // PROCESSTIME_H
