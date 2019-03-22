#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QTime>
#include <QVector>
#include <QMap>
#include <QVariant>

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
    Q_PROPERTY(QString partida READ partida WRITE setPartida NOTIFY startTimeChanged)

    Q_PROPERTY(QString invRider READ invRider WRITE setInvRider)
    Q_PROPERTY(QString invCat READ invCat WRITE setInvCat)

    Q_PROPERTY(QString ranking READ ranking )
    Q_PROPERTY(QString flatElapsed READ flatElapsed )
    Q_PROPERTY(QString color READ color)

    Q_PROPERTY(QVariant listRanking READ listRanking NOTIFY listRankingChanged)

public:
    explicit ReadData(QObject *parent = nullptr);

    QString userName() const { return m_userName;}

    QString posicion() const { return m_position;}
    QString nombre() const { return m_name;}
    QString categoria() const { return m_cat;}
    QString club() const { return m_club;}
    QString procedencia() const { return m_home;}
    QString numero() const { return m_num;}
    QString partida() const { return m_StartTime;}
    QString invRider() const { return QString::number(m_intervalRider);}
    QString invCat() const { return QString::number(m_intervalCat);}

    QString ranking() const { return m_ranking;}
    QString flatElapsed() const { return m_flatElapsed;}
    QString color() const { return m_color;}
    QVariant listRanking() const { return m_ListRanking;}


    void setUserName(const QString &userName);
    void setInvRider(const QString &qsInvRider);
    void setInvCat(const QString &qsInvCat);
    void setPartida(const QString &partida);

    Q_INVOKABLE void runDefaults();
    Q_INVOKABLE void readFile();
    Q_INVOKABLE void saveFile(QString);
    Q_INVOKABLE void addResult(QString);
    Q_INVOKABLE void saveResult();
    Q_INVOKABLE void saveStartList();
    Q_INVOKABLE void storageRider(QString);
    Q_INVOKABLE void getColorRider(QString);

    Q_INVOKABLE void createCatResults(int);
    Q_INVOKABLE void getTotalResults();
    Q_INVOKABLE void getCatComboBox();

signals:
    void userNameChanged();
    void riderChanged();
    void startTimeChanged();
    void listRankingChanged(QString task);

private:
    QString m_userName;
    QString m_position;
    QString m_name;
    QString m_cat;
    QString m_club;
    QString m_home;
    QString m_num;
    QString m_StartTime;

    QString m_ranking;
    QString m_flatElapsed;
    QString m_color;

    QTime m_qStart;
    int m_intervalRider = 10;
    int m_intervalCat = 20;
    int m_posStart = 0;

    QList<QString> m_riderRanking;
    QVariant m_ListRanking;

    QVector<QString> m_riderStartTime;
    QVector<QString> m_allCatResultTime;
//    QVector<QString> m_catResultTime;

    QVector<int> m_iCatResultTime;
    QVector<QVector<int>> m_iTotalResultTime;

    QVector<QVector<int>> m_qvData; //Save all the times


    QVector<QString> m_iCatName;
    QVector<QVector<QVector<int>>> m_CatData;

    QMap<int, QString> m_ridersDB;
    int m_iPos = 1;

    QMap<QString, QString> m_allCat;

private:
    void saveData(int, QString);
    void changeNameTime(QString&);
    void generalResult();
    void buildResult();
    void searchNewRanking(int, int);
};

#endif // READDATA_H
