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

    Q_PROPERTY(QString ranking READ ranking )
    Q_PROPERTY(QString flatElapsed READ flatElapsed )
    Q_PROPERTY(QString color READ color)

public:
    explicit ProcessTime(QObject *parent = nullptr);

    QString elapsed() const { return m_elapsed;}
    QString ranking() const { return m_ranking;}
    QString flatElapsed() const { return m_flatElapsed;}
    QString color() const { return m_color;}

    Q_INVOKABLE void setTimeRef(QString);
    Q_INVOKABLE int getDiff(QString);
    Q_INVOKABLE int getPos(QString);
    Q_INVOKABLE QString getTime(QString, QString);
    Q_INVOKABLE QString getRunTime(QString, QString);
    Q_INVOKABLE void cleanRanking();
    Q_INVOKABLE int getDiffSeg(QString, QString);
    Q_INVOKABLE void getColor(QString);

signals:
    void elapsedChanged();

private:
    QString m_elapsed;
    QString m_ranking;
    QString m_flatElapsed;
    QString m_color;

    QTime m_firstTime;
    QVector<int> integerVector;    
};

#endif // PROCESSTIME_H
