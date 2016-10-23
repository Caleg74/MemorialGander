#include "dbifacebase.h"
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

dbIfaceBase::dbIfaceBase()
: dBConnection()
{
    QDate qDate = QDate::currentDate();
    m_iCurrentYear = qDate.year();
}

void dbIfaceBase::getCountriesList(QStringList* p_pList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT nicename FROM nations");

        *p_pList << "Nazione..";
        while (query.next())
        {
            QString name = query.value(0).toString();
            *p_pList << name;
        }

        qSort(*p_pList);
    }
    else
    {
        qInfo() << "dbInterface::getCountriesList(): Db not initialized";
    }
}

void dbIfaceBase::getRegisterdGymnastList(QStringList* p_pList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT first_name, last_name, gender, nation_id FROM athlete");

        while(query.next())
        {
            // Convert NationId to nicename
            QString strNationName = getNationName(query.value(3).toInt(), NI_IocName);

            *p_pList << query.value(0).toString().trimmed() + ", "
                    + query.value(1).toString().trimmed() + ", ("
                    + strNationName.trimmed() + ")";
        }

        qSort(*p_pList);

        // put it at the beginning
        p_pList->insert(p_pList->begin(), "Seleziona ginnasti..");
    }
    else
    {
        qInfo() << "dbInterface::getRegisterdGymnastList(): Db not initialized";
    }
}

void dbIfaceBase::getEventSelectedGymnastList(QStringList* p_pList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT first_name, last_name, gender, nation_id FROM event_athlete_vw");

        while(query.next())
        {
            QString countryIoc = getNationName(query.value(3).toString().trimmed().toInt(), dbIfaceBase::NI_IocName);
            *p_pList << query.value(0).toString().trimmed() + ", "
                    + query.value(1).toString().trimmed() + ", ("
                    + countryIoc + ")";
        }

//        qSort(*p_pList);

        // put it at the beginning
        p_pList->insert(p_pList->begin(), "Seleziona ginnasti..");
    }
    else
    {
        qInfo() << "dbInterface::getEventSelectedGymnastList(): Db not initialized";
    }
}

int dbIfaceBase::getNationId(QString& p_strNiceName)
{
    int iId = 0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.exec("SELECT id,nicename FROM nations WHERE nicename = '" + p_strNiceName + "'");

        if (query.first())
        {
            iId = query.value(0).toInt();
        }
        else
        {
            qCritical() << "No id found for nation: " << p_strNiceName;
        }
    }
    else
    {
        qInfo() << "dbInterface::getNationId(): Db not initialized";
    }

    return iId;
}

QString dbIfaceBase::getNationName(int p_iNationId, enum NationInfo infoType)
{
    QString strName;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.exec("SELECT id,nicename,ioc,iso FROM nations WHERE id = '" + QString::number(p_iNationId, 10) + "'");

        if (query.first())
        {
            int iValNbr = 1;
            switch(infoType)
            {
            case NI_Nicename:  iValNbr = 1; break;
            case NI_IocName:   iValNbr = 2; break;
            case NI_IsoName:   iValNbr = 3; break;

            default: qWarning() << "Invalid nation info " << infoType;
            }

            strName = query.value(iValNbr).toString();
        }
        else
        {
            qCritical() << "No name found for Id: " << p_iNationId;
        }
    }
    else
    {
        qInfo() << "dbInterface::getNationName(): Db not initialized";
    }

    return strName;
}



int dbIfaceBase::getGymnastId(QString& p_firstName, QString& p_lastName)
{
    int athleteId = 0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.exec("SELECT id, first_name, last_name FROM athlete WHERE first_name = '" + p_firstName + "'"
                   " AND last_name = '" + p_lastName + "'");

        if (query.first())
        {
            athleteId = query.value(0).toInt();
        }
        else
        {
            qCritical() << "No Id found for : " << p_firstName << " "  << p_lastName;
        }
    }
    else
    {
        qInfo() << "dbInterface::getGymnastId(): Db not initialized";
    }

    return athleteId;
}

int dbIfaceBase::getApparatusId(QString& p_apparatusName, QString& p_gender)
{
    int iApparatusId = 0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        QString strQuery = "SELECT id, name_it FROM apparatus WHERE name_it = '"
                + p_apparatusName + "' AND gender = '"
                + p_gender + "'";

        query.exec(strQuery);

        if (query.first())
        {
            iApparatusId = query.value(0).toInt();
        }
        else
        {
            qCritical() << "No Id found for: " << p_apparatusName << ", " << p_gender;
        }
    }
    else
    {
        qInfo() << "dbInterface::getApparatusId(): Db not initialized";
    }

    return iApparatusId;
}


QString dbIfaceBase::getGender(int p_iAthleteId)
{
    QString strGender;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.exec("SELECT id, gender FROM athlete WHERE id = " + QString::number(p_iAthleteId, 10));

        if (query.first())
        {
            strGender = query.value(1).toString();
        }
        else
        {
            qCritical() << "No gender found for (athleteId)= " << p_iAthleteId;
        }
    }
    else
    {
        qInfo() << "dbInterface::getGender(): Db not initialized";
    }

    return strGender;
}


void dbIfaceBase::retrieveRegisteredGymnastList(QList<QStringList>& p_strGymnList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT first_name, last_name, gender, nation_id FROM athlete");

        while(query.next())
        {
            QStringList strData;
            strData << query.value(0).toString().trimmed()  // first name
                    << query.value(1).toString().trimmed()  // last name
                    << query.value(3).toString().trimmed()  // nation_id
                    << query.value(2).toString().trimmed(); // gender

            p_strGymnList << strData;
            qInfo() << "Athlete retrieved: " << strData;
        }
    }
    else
    {
        qInfo() << "dbInterface::retrieveRegisteredGymnastList(): Db not initialized";
    }
}

void dbIfaceBase::retrieveGymnastEventList(QList<QStringList>& p_strGymnList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT first_name, last_name, gender, nation_id FROM event_athlete_vw");

        while(query.next())
        {
            QStringList strData;
            strData << query.value(0).toString().trimmed()  // first name
                    << query.value(1).toString().trimmed()  // last name
                    << query.value(3).toString().trimmed()  // nation_id
                    << query.value(2).toString().trimmed(); // gender

            p_strGymnList << strData;
            qInfo() << "Athlete retrieved: " << strData;
        }
    }
    else
    {
        qInfo() << "dbInterface::retrieveGymnastEventList(): Db not initialized";
    }
}

int dbIfaceBase::getCurrentEventId()
{
    int eventId = 0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.exec("SELECT id, year FROM sport_event");

        while (query.next())
        {
            if (query.value(1).toString().startsWith(QString::number(m_iCurrentYear, 10)))
            {
                eventId = query.value(0).toInt();
            }
        }

        if (eventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getCurrentEventId(): Db not initialized";
    }

    return eventId;
}

bool dbIfaceBase::isScorePresent(const int p_iEventId,
                                 const int p_iAthleteId,
                                 const int p_iApparatusId)
{
    bool bEntryPresent = true;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        QString strQuery = "SELECT sport_event_id, athlete_id, apparatus_id FROM scores WHERE"
                   " sport_event_id = "   + QString::number(p_iEventId, 10) +
                   " AND athlete_id = "   + QString::number(p_iAthleteId, 10)+
                   " AND apparatus_id = " + QString::number(p_iApparatusId, 10);

        query.exec(strQuery);
        bEntryPresent = query.first();


        if (bEntryPresent)
            qInfo() << "Score present";
        else
            qInfo() << "Score NOT present";

    }
    else
    {
        qInfo() << "dbInterface::isScorePresent(): Db not initialized";
    }

    return bEntryPresent;
}

void dbIfaceBase::getApparatusList(QStringList* p_pList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.exec("SELECT id, name_it, name_en, gender FROM apparatus");

        while (query.next())
        {
            QString name = query.value(0).toString() + ","  // id
                         + query.value(1).toString() + ","  // name_it
                         + query.value(2).toString() + ","  // name_en
                         + query.value(3).toString();       // gender
            *p_pList << name;
        }
    }
    else
    {
        qInfo() << "dbInterface::getApparatusList(): Db not initialized";
    }
}

float dbIfaceBase::getStartScore(const int p_iAthleteId, const int p_iApparatusId)
{
    float fScore = 0.0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        int iEventId = getCurrentEventId();

         QString strQuery = "SELECT start_score FROM scores WHERE "
                   " sport_event_id = "   + QString::number(iEventId, 10) +
                   " AND athlete_id = "   + QString::number(p_iAthleteId, 10)+
                   " AND apparatus_id = " + QString::number(p_iApparatusId, 10);

         query.exec(strQuery);

        while (query.next())
        {
//            qDebug() << query.value(0).typeName()
//                    << ", " << query.value(0).toString();
            fScore = query.value(0).toFloat();
        }

        if (iEventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getStartScore(): Db not initialized";
    }

    return fScore;
}

float dbIfaceBase::getFinalScore(const int p_iAthleteId, const int p_iApparatusId)
{
    float fScore = 0.0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        int iEventId = getCurrentEventId();

        QString strQuery = "SELECT final_score FROM scores WHERE "
                  " sport_event_id = "   + QString::number(iEventId, 10) +
                  " AND athlete_id = "   + QString::number(p_iAthleteId, 10)+
                  " AND apparatus_id = " + QString::number(p_iApparatusId, 10);

        query.exec(strQuery);

        while (query.next())
        {
            fScore = query.value(0).toFloat();
        }

        if (iEventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getFinalScore(): Db not initialized";
    }

    return fScore;
}

AllScores dbIfaceBase::getAllScores(const int p_iAthleteId, const int p_iApparatusId)
{
    AllScores allScores = {0.0, 0.0, 0, false};

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        int iEventId = getCurrentEventId();

        QString strQuery = "SELECT start_score, final_score, force_score, final_apparatus FROM scores WHERE "
                  " sport_event_id = "   + QString::number(iEventId, 10) +
                  " AND athlete_id = "   + QString::number(p_iAthleteId, 10)+
                  " AND apparatus_id = " + QString::number(p_iApparatusId, 10);

        query.exec(strQuery);

        while (query.next())
        {
            allScores.StartScore = query.value(0).toFloat();
            allScores.FinalScore = query.value(1).toFloat();
            allScores.ForceScore = query.value(2).toInt();
            allScores.IsFinalApparatus = query.value(3).toBool();
        }

        if (iEventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getAllScores(): Db not initialized";
    }

    return allScores;
}

int dbIfaceBase::getForceScore(const int p_iAthleteId, const int p_iApparatusId)
{
    int iForceScore = 0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        int iEventId = getCurrentEventId();

        QString strQuery = "SELECT force_score FROM scores WHERE "
                  " sport_event_id = "   + QString::number(iEventId, 10) +
                  " AND athlete_id = "   + QString::number(p_iAthleteId, 10)+
                  " AND apparatus_id = " + QString::number(p_iApparatusId, 10);

        query.exec(strQuery);

        while (query.next())
        {
            iForceScore = query.value(0).toInt();
        }

        if (iEventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getForceScore(): Db not initialized";
    }

    return iForceScore;
}
