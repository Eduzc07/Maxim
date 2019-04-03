#include "include/readdata.h"

#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QTimer>

ReadData::ReadData(QObject *parent) :
    QObject(parent)
{
    m_riderStartTime.append("Lugar,Nombre,Categoria,Club,Procedencia,Numero,Hora de Partida");
    m_allCatResultTime.append("Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia");

    //All the categories
    m_allCat.insert("ED", "Elite Dobles");
    m_allCat.insert("ER", "Elite Rigidas");
    m_allCat.insert("M", "Master");
    m_allCat.insert("C", "Cadetes");
    m_allCat.insert("N", "Noveles");
    m_allCat.insert("J", "Junior");
    m_allCat.insert("D", "Damas");
    m_allCat.insert("EO", "Enduro");
    m_allCat.insert("G", "General");
}

void ReadData::runDefaults()
{
    QTime nextTime = QTime().currentTime();
    QTime t = nextTime.addSecs(1*60); //30 min
    m_qStart.setHMS(t.hour(), t.minute(), 0);
    m_StartTime = m_qStart.toString("hh:mm:ss");
    emit startTimeChanged();
}

void ReadData::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

void ReadData::setInvRider(const QString &qsInvRider)
{
    if (qsInvRider.toInt() == m_intervalRider)
        return;

    m_intervalRider = qsInvRider.toInt();
}

void ReadData::setInvCat(const QString &qtInvCat)
{
    if (qtInvCat.toInt() == m_intervalCat)
        return;

    m_intervalCat = qtInvCat.toInt();
}

void ReadData::setPartida(const QString &partida)
{
    if (partida == m_StartTime)
        return;

    m_qStart = QTime::fromString(partida);
}

void ReadData::readFile(QString inputFile )
{
    auto fileN = inputFile.split("//");
    QString name = fileN[1];
    QFile file(name);

//    QFile file(":/lista.csv");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return;
    }

    //Write Init Time
    m_StartTime = m_qStart.toString("hh:mm:ss");
    m_posStart = 0;

    QTextStream xin(&file);
    while(!xin.atEnd()){
        auto line = xin.readLine();
        auto values = line.split(",");
        const int colCount = values.size();

//        qDebug()<<"---------"<< values;
        //Jump the first line
        int fRow = QString::compare(values[0], "Lugar", Qt::CaseInsensitive);  // if strings are equal x should return 0
        int emptyRow = QString::compare(values[0], "", Qt::CaseInsensitive);  // if strings are equal x should return 0
        if (fRow == 0)
            continue;

        for(int jx = 0; jx < colCount;jx++){
            saveData(jx, values.at(jx));
//            qDebug()<< xin.pos() << "-------------------"<< values.at(jx);
        }

        if (emptyRow == 0){
            m_StartTime = "";
            m_posStart += (m_intervalCat/m_intervalRider - 1);
        } else {
            QTime nextTime = m_qStart.addSecs(m_posStart*m_intervalRider);
            m_StartTime = nextTime.toString("hh:mm:ss");
//            changeNameTime(m_StartTime);
//            qDebug()<< "====================tt" << m_StartTime;
            //Add new rider
            m_posStart++;
        }

        //Save vector to write File
        m_riderStartTime.append(line + "," + m_StartTime);

        //Create DB
        int num = m_num.toInt();
        QString data = QString("%1,%2,%3,%4,%5").arg(m_name)
                .arg(m_cat)
                .arg(m_club)
                .arg(m_home)
                .arg(m_num);
        m_ridersDB.insert(num, data);

        emit riderChanged();
    }

    //Final round 30 minutes later as default
    QTime nextTime =  m_qStart.addSecs(m_posStart*m_intervalRider);
    QTime t = nextTime.addSecs(30*60); //30 min
    m_qStart.setHMS(t.hour(), t.minute(), 0);
    m_StartTime = m_qStart.toString("hh:mm:ss");

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

void ReadData::saveFile(QString filename)
{
    auto fileN = filename.split("//");
    QString name = fileN[1];

    QFile file(name);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream( &file );
        for (int i = 0; i < m_riderStartTime.size(); ++i) {
            stream << m_riderStartTime.at(i) << endl;
        }
    }
    file.close();
}

void ReadData::addResult(QString result)
{
    auto values = result.split(",");
    //Save all the cat if it is empty
    int emptyRow = QString::compare(values[0], "", Qt::CaseInsensitive);  // if strings are equal x should return 0
    if (emptyRow == 0){
        m_iTotalResultTime.append(m_iCatResultTime);
        for (int i = 0; i < m_iTotalResultTime.size(); ++i) {
            qDebug()<< "==>" << m_iTotalResultTime.at(i);
        }
        m_allCatResultTime.append(",,,,,,,");
        m_iPos=1;

        m_iCatResultTime.clear();
//        m_catResultTime.clear();
        return;
    }

    int number = values[0].toInt();
    QString time = values[1] + "," + values[2] ;
    QTime currentTime = QTime::fromString("00:" + values[1]);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);

//    "Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia"
    QString data = QString("%1,%2,%3").arg(m_iPos)
            .arg(m_ridersDB.value(number))
            .arg(time);
    m_allCatResultTime.append(data);
    m_iPos++;

    m_iCatResultTime.append(number); //StarList
    qDebug()<< "===================="<< number << " :" << time<< " :" << t_ms;

//    m_catResultTime.append(values[1]);

    //Save All the Results
    QVector<int> timeChrono = {t_ms, number};
    m_qvData.append(timeChrono);
}


void ReadData::saveResult(QString filename)
{
    auto fileN = filename.split("//");
    QString name = fileN[1];
    QFile file(name);
//    QFile file("Result.csv");


    buildResult();
    generalResult();
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream( &file );
        for (int i = 0; i < m_allCatResultTime.size(); ++i) {
            stream << m_allCatResultTime.at(i) << endl;
        }
    }
    file.close();
}

void ReadData::generalResult()
{
    m_allCatResultTime.append(",,,,,,,");
    m_iPos=1;

    std::sort(m_qvData.begin(),m_qvData.end() ,[](const QVector<int>& left,const QVector<int>& right)->bool{
                if(left.empty() && right.empty())
                    return false;
                if(left.empty())
                    return true;
                if(right.empty())
                    return false;
                return left.first()<right.first();
            }
        );

    QTime time0;
    QString time;
    QString data;
    //.N.S.P.
    if (m_qvData.at(0).at(0) == (59*60*1000)){
        data = QString("%1,%2,%3").arg("-")
                .arg(m_ridersDB.value(m_qvData.at(0).at(1)))
                .arg("N.S.P., ");
    } else {
        time0 = QTime::fromMSecsSinceStartOfDay(m_qvData.at(0).at(0));
        time = time0.toString("mm:ss.zzz") + ", ";
        data = QString("%1,%2,%3").arg(1)
                .arg(m_ridersDB.value(m_qvData.at(0).at(1)))
                .arg(time);
    }

    m_allCatResultTime.append(data);

    for (int i = 1; i < m_qvData.size(); ++i) {
        int number = m_qvData.at(i).at(1);
        //.N.S.P.
        if (m_qvData.at(i).at(0) == (59*60*1000)){
            data = QString("%1,%2,%3").arg("-")
                    .arg(m_ridersDB.value(number))
                    .arg("N.S.P., ");
        } else {
            QTime time1 = QTime::fromMSecsSinceStartOfDay(m_qvData.at(i).at(0));;
            time = time1.toString("mm:ss.zzz");
            qint64 millisecondsDiff = time0.msecsTo(time1);
            int t_ms = static_cast<int>(millisecondsDiff);

            time += ",+" + QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss.zzz");
            //    "Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia"
            data = QString("%1,%2,%3").arg(i+1)
                    .arg(m_ridersDB.value(number))
                    .arg(time);
        }

        m_allCatResultTime.append(data);
    }
}

void ReadData::saveStartList(QString filename)
{
    m_riderStartTime.clear();
    m_riderStartTime.append("Lugar,Nombre,Categoria,Club,Procedencia,Numero,Hora de Partida");

    m_StartTime = m_qStart.toString("hh:mm:ss");
    m_posStart = 0;

    //Read Categories
    for (int j = 0; j < m_CatData.size(); j++) {
         int lugar = 1;
        for (int i = m_CatData.at(j).size() - 1; i >= 0; i--) {

            QTime nextTime = m_qStart.addSecs(m_posStart*m_intervalRider);
            m_StartTime = nextTime.toString("hh:mm:ss");
            int number = m_CatData.at(j).at(i).at(1);

            //Save vector to write File
            QString data = QString("%1,%2,%3").arg(lugar)
                    .arg(m_ridersDB.value(number))
                    .arg(m_StartTime);

            m_riderStartTime.append(data);
            //Add new rider
            m_posStart++;
            lugar++;
        }
        m_posStart += (m_intervalCat/m_intervalRider - 1);

        if (j < m_CatData.size() - 1)
            m_riderStartTime.append(",,,,,,");
    }

    //"file://StartList.csv"
    saveFile(filename);
}

void ReadData::storageRider(QString result)
{
    qDebug()<< "==========result=========="<< result;
    auto values = result.split(",");
    int number = values[0].toInt(); //Save Numebr
    QString time = values[1];       //Save time
    QString cat = values[2];        //Save cat

    QTime currentTime = QTime::fromString("00:" + time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);

    if (m_iCatName.size() == 0) {
        m_iCatName.append(cat);
        QVector<int> timeChrono = {t_ms, number};
        QVector<QVector<int>> cat;
        cat.append(timeChrono);
        m_CatData.append(cat);

        //First rider first Category
        m_ranking = "1";
        m_flatElapsed = "";
        m_color = "green";
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

        //First rider new Category
        m_ranking = "1";
        m_flatElapsed = "";
        m_color = "green";
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

    searchNewRanking(number, posCat);
}

void ReadData::searchNewRanking(int num, int cat)
{
    QVector<QVector<int>> categoryTime = m_CatData.value(cat);

    int newPos = 0;
    for (newPos = 1; newPos < categoryTime.size(); newPos++) {
        if (num == categoryTime.at(newPos-1).at(1))
            break;
    }

    //If is the best one
    if (newPos == 1){
        m_ranking = "1";
        m_flatElapsed = "";
        m_color = "green";
        return;
    }

    //Result rider
    m_ranking = QString::number(newPos); //New Position

    int bestTime = categoryTime.at(0).at(0);
    int currenTime = categoryTime.at(newPos-1).at(0);

    int diff = currenTime - bestTime;
    QString sign = "+";
    m_color = "red";

    if (diff < 0){
        diff = bestTime - currenTime;
        sign = "-";
        m_color = "green";
    }

    QString qTime = QTime::fromMSecsSinceStartOfDay(diff).toString("mm:ss.zzz");
    m_flatElapsed = sign + qTime; //New difference
}

void ReadData::getColorRider(QString result)
{
    m_ranking = "0";

    auto values = result.split(",");
    QString time = values[0];       //Get time
    QString cat = values[1];        //Get cat

    int posCat = 9999;
    for (int i = 0; i < m_iCatName.size(); ++i) {
        if (m_iCatName.at(i) == cat){
            posCat = i;
            break;
        }
    }

    QTime currentTime = QTime::fromString("00:" + time);
    qint64 millisecondsDiff = currentTime.msecsSinceStartOfDay();
    int t_ms = static_cast<int>(millisecondsDiff);

    //New Category
    if (posCat == 9999){
        if (t_ms <= 0)
            m_color = "gray";
        else
            m_color = "green";

        return;
    }

    //Get best time for this category
    QVector<QVector<int>> categoryTime = m_CatData.value(posCat);
    int bestTime = categoryTime.at(0).at(0);

    m_color = "red";

    if (t_ms <= bestTime)
        m_color = "green";

    if (t_ms <= 0)
        m_color = "gray";

    //Get current Position
    QVector<int> timeChrono = {t_ms, 0};
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

    int newPos = 0;
    for (newPos = 0; newPos < categoryTime.size(); newPos++) {
        if (0 == categoryTime.at(newPos).at(1))
            break;
    }

    //Result rider
    m_ranking = QString::number(newPos); //New Position
}

void ReadData::buildResult()
{
    m_allCatResultTime.clear();
    m_qvData.clear();

    for (int j = 0; j < m_CatData.size(); ++j) {
        QTime time0;
        QString time;
        QString data;

        //.N.S.P.
        if (m_CatData.at(j).at(0).at(0) == (59*60*1000)){
            data = QString("%1,%2,%3").arg("-")
                    .arg(m_ridersDB.value(m_CatData.at(j).at(0).at(1)))
                    .arg("N.S.P., ");
        } else {

            qDebug() << m_ridersDB.value(m_CatData.at(j).at(0).at(1));
            time0 = QTime::fromMSecsSinceStartOfDay(m_CatData.at(j).at(0).at(0));
            time = time0.toString("mm:ss.zzz") + ", ";
            data = QString("%1,%2,%3").arg(1)
                    .arg(m_ridersDB.value(m_CatData.at(j).at(0).at(1)))
                    .arg(time);
        }
        m_allCatResultTime.append(data);
        //Save All the Results
        QVector<int> timeChrono = {m_CatData.at(j).at(0)};
        m_qvData.append(timeChrono);

        for (int i = 1; i < m_CatData.at(j).size(); ++i) {
            int number = m_CatData.at(j).at(i).at(1);
//            qDebug() << "============> " << m_ridersDB.value(number);

            //.N.S.P.
            if (m_CatData.at(j).at(i).at(0) == (59*60*1000)){
                data = QString("%1,%2,%3").arg("-")
                        .arg(m_ridersDB.value(number))
                        .arg("N.S.P., ");
            } else {


                QTime time1 = QTime::fromMSecsSinceStartOfDay(m_CatData.at(j).at(i).at(0));
                time = time1.toString("mm:ss.zzz");
                qint64 millisecondsDiff = time0.msecsTo(time1);
                int t_ms = static_cast<int>(millisecondsDiff);
                time += ",+" + QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss.zzz");
                //    "Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia"
                data = QString("%1,%2,%3").arg(i+1)
                        .arg(m_ridersDB.value(number))
                        .arg(time);
            }

            m_allCatResultTime.append(data);

            //Save All the Results
            QVector<int> timeChrono = {m_CatData.at(j).at(i).at(0), number};
            m_qvData.append(timeChrono);
        }
        m_allCatResultTime.append(",,,,,,,");
    }
}

void ReadData::createCatResults(int pos)
{
    //Clear List
    m_riderRanking.clear();
    m_ListRanking.clear();
//    emit listRankingChanged();

    //If there is not a category, is all results
    if (pos == 999){
        getTotalResults();
        return;
    }

    QTime time0;
    QString time;
    QString data;

    //.N.S.P.
    if (m_CatData.at(pos).at(0).at(0) == (59*60*1000)){
        data = QString("%1,%2,%3").arg("-")
                .arg(m_ridersDB.value(m_CatData.at(pos).at(0).at(1)))
                .arg("N.S.P., ");
    }else{
        time0 = QTime::fromMSecsSinceStartOfDay(m_CatData.at(pos).at(0).at(0));
        time = time0.toString("mm:ss.zzz") + ", ";
        data = QString("%1,%2,%3").arg(1)
                .arg(m_ridersDB.value(m_CatData.at(pos).at(0).at(1)))
                .arg(time);
    }

    //Append!!!!
    m_riderRanking.append(data);

    for (int i = 1; i < m_CatData.at(pos).size(); ++i) {
        int number = m_CatData.at(pos).at(i).at(1);
        //.N.S.P.
        if (m_CatData.at(pos).at(i).at(0) == (59*60*1000)){
            data = QString("%1,%2,%3").arg("-")
                    .arg(m_ridersDB.value(number))
                    .arg("N.S.P., ");
        } else {
            QTime time1 = QTime::fromMSecsSinceStartOfDay(m_CatData.at(pos).at(i).at(0));;
            time = time1.toString("mm:ss.zzz");
            qint64 millisecondsDiff = time0.msecsTo(time1);
            int t_ms = static_cast<int>(millisecondsDiff);
            time += ",+" + QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss.zzz");
            //    "Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia"
            data = QString("%1,%2,%3").arg(i+1)
                    .arg(m_ridersDB.value(number))
                    .arg(time);
        }
        m_riderRanking.append(data);

    }
    m_ListRanking = QVariant::fromValue(m_riderRanking);
    emit listRankingChanged("Result");
}

void ReadData::getTotalResults()
{
    m_qvData.clear();
    m_riderRanking.clear();
    m_ListRanking.clear();

    for (int j = 0; j < m_CatData.size(); ++j) {
        //Save All the Results
        QVector<int> timeChrono = {m_CatData.at(j).at(0)};
        m_qvData.append(timeChrono);
        for (int i = 1; i < m_CatData.at(j).size(); ++i) {
            int number = m_CatData.at(j).at(i).at(1);
            //Save All the Results
            QVector<int> timeChrono = {m_CatData.at(j).at(i).at(0), number};
            m_qvData.append(timeChrono);
        }
    }

    m_iPos = 1;

    std::sort(m_qvData.begin(),m_qvData.end() ,[](const QVector<int>& left,const QVector<int>& right)->bool{
                if(left.empty() && right.empty())
                    return false;
                if(left.empty())
                    return true;
                if(right.empty())
                    return false;
                return left.first()<right.first();
            }
        );

    QTime time0;
    QString time;
    QString data;
    //.N.S.P.
    if (m_qvData.at(0).at(0) == (59*60*1000)){
        data = QString("%1,%2,%3").arg("-")
                .arg(m_ridersDB.value(m_qvData.at(0).at(1)))
                .arg("N.S.P., ");
    }else{
        time0 = QTime::fromMSecsSinceStartOfDay(m_qvData.at(0).at(0));
        time = time0.toString("mm:ss.zzz") + ", ";
        data = QString("%1,%2,%3").arg(1)
                .arg(m_ridersDB.value(m_qvData.at(0).at(1)))
                .arg(time);
    }

    m_riderRanking.append(data);

    for (int i = 1; i < m_qvData.size(); ++i) {
        int number = m_qvData.at(i).at(1);
        //.N.S.P.
        if (m_qvData.at(i).at(0) == (59*60*1000)){
            data = QString("%1,%2,%3").arg("-")
                    .arg(m_ridersDB.value(number))
                    .arg("N.S.P., ");
        } else {
            QTime time1 = QTime::fromMSecsSinceStartOfDay(m_qvData.at(i).at(0));;
            time = time1.toString("mm:ss.zzz");
            qint64 millisecondsDiff = time0.msecsTo(time1);
            int t_ms = static_cast<int>(millisecondsDiff);

            time += ",+" + QTime::fromMSecsSinceStartOfDay(t_ms).toString("mm:ss.zzz");
            //    "Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia"
            data = QString("%1,%2,%3").arg(i+1)
                    .arg(m_ridersDB.value(number))
                    .arg(time);
        }

        m_riderRanking.append(data);
    }
    m_ListRanking = QVariant::fromValue(m_riderRanking);
    emit listRankingChanged("Result");
}

void ReadData::getCatComboBox()
{
    m_riderRanking.clear();
    m_ListRanking.clear();

    for (int i = 0; i < m_iCatName.size(); ++i) {
        QString data = QString("%1,%2")
                .arg(m_allCat[m_iCatName.at(i)])
                .arg(i);
        m_riderRanking.append(data);
    }

    QString data = QString("%1,%2")
            .arg(m_allCat["G"])
            .arg(999);
    m_riderRanking.append(data);

    m_ListRanking = QVariant::fromValue(m_riderRanking);
    emit listRankingChanged("CatMenu");
}

void ReadData::clearData()
{
    m_riderRanking.clear();
    m_ListRanking.clear();

    m_riderRanking.clear();
    m_ListRanking.clear();

    m_riderStartTime.clear();
    m_allCatResultTime.clear();

    m_iCatResultTime.clear();
    m_iTotalResultTime.clear();

    m_qvData.clear(); //Save all the times

    m_iCatName.clear();
    m_CatData.clear();

    m_ridersDB.clear();
    m_iPos = 1;

    m_riderStartTime.append("Lugar,Nombre,Categoria,Club,Procedencia,Numero,Hora de Partida");
    m_allCatResultTime.append("Lugar,Nombre,Categoria,Club,Procedencia,Numero,Tiempo Final,Diferencia");
}


