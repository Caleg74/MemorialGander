#include "dbifacebase.h"
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "apparatuslist.h"

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

        p_pList->sort(Qt::CaseInsensitive);
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
            QString firstName = query.value(0).toString().trimmed();
            QString lastName = query.value(1).toString().trimmed();

            *p_pList << firstName + ", "  +  lastName + ", ("
                    + strNationName.trimmed() + ")";
        }

        p_pList->sort(Qt::CaseInsensitive);

        // put it at the beginning
        p_pList->insert(p_pList->begin(), "Seleziona ginnasti..");
    }
    else
    {
        qInfo() << "dbInterface::getRegisterdGymnastList(): Db not initialized";
    }
}

void dbIfaceBase::getEventSelectedGymnastList(QStringList* p_pList, bool p_bMen, bool p_bWomen)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        int iEventId = getCurrentEventId();

        QString strQuery;
        QString gender;
        if (p_bMen && p_bWomen)
        {
            strQuery = "SELECT first_name, last_name, gender, nation_id FROM event_athlete_vw WHERE"
                       " sport_event_id = ?";
        }
        else if (p_bMen)
        {
            strQuery = "SELECT first_name, last_name, gender, nation_id FROM event_athlete_vw WHERE"
                       " sport_event_id = ? AND gender = ?";
            gender = "M";
        }
        else if (p_bWomen)
        {
            strQuery = "SELECT first_name, last_name, gender, nation_id FROM event_athlete_vw WHERE"
                       " sport_event_id = ? AND gender = ?";
            gender = "F";
        }
        else
        {
            *p_pList << "Seleziona ginnasti..";
            return;
        }

        query.prepare(strQuery);
        query.addBindValue(iEventId);
        if (!gender.isEmpty())
            query.addBindValue(gender);
        query.exec();

        while(query.next())
        {
            QString firstName = query.value(0).toString().trimmed();
            QString lastName = query.value(1).toString().trimmed();
            QString countryIoc = getNationName(query.value(3).toString().trimmed().toInt(), dbIfaceBase::NI_IocName);
            *p_pList << firstName + ", " + lastName + ", ("
                    + countryIoc + ")";
        }

//        qSort(*p_pList);
//        qInfo() << *p_pList;

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

        query.prepare("SELECT id, nicename FROM nations WHERE nicename = ?");
        query.addBindValue(p_strNiceName);
        query.exec();

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

        query.prepare("SELECT id, nicename, ioc, iso FROM nations WHERE id = ?");
        query.addBindValue(p_iNationId);
        query.exec();

        if (query.first())
        {
            int iValNbr = 1;
            switch(infoType)
            {
            case NI_Nicename:  iValNbr = 1; break;
            case NI_IocName:   iValNbr = 2; break;
            case NI_IsoName:   iValNbr = 3; break;

//            default: qWarning() << "Invalid nation info " << infoType;  // already covered all of them
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

        query.prepare("SELECT id, first_name, last_name FROM athlete "
                      "WHERE first_name = ? AND last_name = ?");
        query.addBindValue(p_firstName);
        query.addBindValue(p_lastName);
        query.exec();

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

        query.prepare("SELECT id, name_it FROM apparatus WHERE name_it = ? AND gender = ?");
        query.addBindValue(p_apparatusName);
        query.addBindValue(p_gender);
        query.exec();

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

        query.prepare("SELECT id, gender FROM athlete WHERE id = ?");
        query.addBindValue(p_iAthleteId);
        query.exec();

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

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT first_name, last_name, gender, nation_id "
                      "FROM event_athlete_vw WHERE sport_event_id = ?");
        query.addBindValue(iEventId);
        query.exec();

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
            if (query.value(1).toString().startsWith(QString::number(m_iCurrentYear, 10)))    // ORI
//            if (query.value(1).toString().startsWith(QString::number(2026, 10)))    // just for test
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

        query.prepare("SELECT sport_event_id, athlete_id, apparatus_id FROM scores "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(p_iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);
        query.exec();
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

float dbIfaceBase::getDifficultyScore(const int p_iAthleteId, const int p_iApparatusId)
{
    float fScore = 0.0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT difficulty_score FROM scores "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);
        query.exec();

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
        qInfo() << "dbInterface::getDifficultyScore(): Db not initialized";
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

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT final_score FROM scores "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);
        query.exec();

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
    AllScores allScores = {0.0, 0.0, 0.0, 0.0, 0, false};

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT difficulty_score, execution_score, penalty_score, final_score, "
                      "force_score, final_apparatus FROM scores "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);
        query.exec();

        while (query.next())
        {
            allScores.DifficultyScore = query.value(0).toFloat();
            allScores.ExecutionScore = query.value(1).toFloat();
            allScores.PenaltyScore = query.value(2).toFloat();
            allScores.FinalScore = query.value(3).toFloat();
            allScores.ForceScore = query.value(4).toInt();
            allScores.IsFinalApparatus = query.value(5).toBool();
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

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT force_score FROM scores "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);
        query.exec();

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

float dbIfaceBase::getAllroundTotalScore(const int p_iAthleteId)
{
    float fTotalScore = 0.0;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        const int iEventId = getCurrentEventId();

        query.prepare("SELECT total_final_score FROM total_scores_vw "
                      "WHERE sport_event_id = ? AND athlete_id = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_iAthleteId);
        query.exec();

        while (query.next())
        {
            fTotalScore = query.value(0).toFloat();
        }

        if (iEventId == 0)
        {
            qCritical() << "No Id found for event year: " << m_iCurrentYear;
        }
    }
    else
    {
        qInfo() << "dbInterface::getAllroundTotalScore(): Db not initialized";
    }

    return fTotalScore;
}

void dbIfaceBase::retrieveChronologicalList(QList<QStringList>& p_strChronoList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        const int iEventId = getCurrentEventId();

        query.prepare("SELECT id, gender, fullname, nation, apparatus, difficulty_score, "
                      "execution_score, penalty_score, final_score, total_score "
                      "FROM chrono_list_vw WHERE event_id = ?");
        query.addBindValue(iEventId);
        query.exec();

        while (query.next())
        {
            QStringList strData;
            strData << query.value(0).toString().trimmed()  // athelete id
                    << query.value(1).toString().trimmed()  // gender
                    << query.value(2).toString().trimmed()  // fullname
                    << query.value(3).toString().trimmed()  // nation
                    << query.value(4).toString().trimmed()  // apparatus
                    << query.value(5).toString().trimmed()  // difficulty_score
                    << query.value(6).toString().trimmed()  // execution_score
                    << query.value(7).toString().trimmed()  // penalty_score
                    << query.value(8).toString().trimmed()  // final_score
                    << query.value(9).toString().trimmed(); // total_score

            p_strChronoList << strData;
//            qInfo() << "Athlete retrieved: " << strData;
        }
    }
    else
    {
        qInfo() << "dbInterface::retrieveChronologicalList(): Db not initialized";
    }
}


bool dbIfaceBase::getLatestScore(const QString p_strGender, int* p_pAthleteId, int* p_pApparatusId)
{
    bool bRet = false;
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        const int iEventId = getCurrentEventId();

        query.prepare("SELECT id, apparatus, final_score FROM chrono_list_vw "
                      "WHERE event_id = ? AND gender = ?");
        query.addBindValue(iEventId);
        query.addBindValue(p_strGender);
        query.exec();

        if (query.first())
        {
            *p_pAthleteId = query.value(0).toString().trimmed().toInt();
            QString strApparatusName = query.value(1).toString().trimmed();
            *p_pApparatusId = ApparatusList::Instance()->getApparatusId(strApparatusName, p_strGender);

            // Check if score is >0 (to avoid non-init issues when list is empty)
            if (query.value(2).toString().trimmed().toFloat() > 0.0f)
            {
                bRet = true;
            }
        }
    }

    return bRet;
}
