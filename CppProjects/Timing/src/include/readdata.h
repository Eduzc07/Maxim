#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QString>
#include <QTime>

class ReadData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString posicion READ posicion)
    Q_PROPERTY(QString nombre READ nombre)
    Q_PROPERTY(QString categoria READ categoria)
    Q_PROPERTY(QString club READ club)
    Q_PROPERTY(QString procedencia READ procedencia)
    Q_PROPERTY(QString numero READ numero)
    Q_PROPERTY(QString partida READ partida)

public:
    explicit ReadData(QObject *parent = nullptr);

    QString userName() const { return m_userName;}

    void setUserName(const QString &userName);

    QString posicion() const { return m_position;}
    QString nombre() const { return m_name;}
    QString categoria() const { return m_cat;}
    QString club() const { return m_club;}
    QString procedencia() const { return m_home;}
    QString numero() const { return m_num;}
    QString partida() const { return m_StartTime;}

    Q_INVOKABLE void readFile();

signals:
    void userNameChanged();
    void riderChanged();

public slots:


private:
    QString m_userName;
    QString m_position;
    QString m_name;
    QString m_cat;
    QString m_club;
    QString m_home;
    QString m_num;
    QString m_StartTime;

    QTime m_qStart;
    int m_interval = 60;
    int m_posStart = 0;

private:
    void saveData(int, QString);
    void changeNameTime(QString&);
};

#endif // READDATA_H
