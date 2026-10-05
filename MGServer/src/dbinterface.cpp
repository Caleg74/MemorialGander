#include "dbinterface.h"
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "messagebox.h"

dbInterface* dbInterface::sm_pInstance = NULL;


dbInterface::dbInterface()
: dbIfaceBase()
{
}

dbInterface* dbInterface::Instance()
{
    if (sm_pInstance == NULL)
    {
        sm_pInstance = new dbInterface();
    }

    return sm_pInstance;
}


int dbInterface::getNationId(QString& p_strNiceName)
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

bool dbInterface::insertGymnast(const QString& p_strFirstName,
                                const QString& p_strLastName,
                                const QString& p_strGender,
                                int p_iNationId)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.prepare("INSERT INTO athlete (first_name, last_name, gender, nation_id) "
                      "VALUES (?, ?, ?, ?)");
        query.addBindValue(p_strFirstName);
        query.addBindValue(p_strLastName);
        query.addBindValue(p_strGender);
        query.addBindValue(p_iNationId);

        const bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "Athlete added: " << p_strFirstName << " " << p_strLastName;
        }
        else
        {
            qCritical() << "Athlete NOT added: " << p_strFirstName << " " << p_strLastName;
            qCritical() << query.lastError();
        }

        return bRet;
    }

    qInfo() << "dbInterface::insertGymnast(): Db not initialized";
    return false;
}

void dbInterface::deleteGymnast(QString& p_strFirstName, QString& p_strLastName)
{

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.prepare("DELETE FROM athlete WHERE first_name = ? AND last_name = ?");
        query.addBindValue(p_strFirstName.trimmed());
        query.addBindValue(p_strLastName.trimmed());
        const bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "Athlete removed: " << p_strFirstName << " " << p_strLastName;
        }
        else
        {
            QString strErr = "Athlete NOT removed: " + p_strFirstName + " " + p_strLastName
                    +  "\n" + query.lastError().text();
            qCritical() << strErr;

            MessageBox cMsgBox;
            cMsgBox.SetTitle("Warning");
            cMsgBox.SetText(strErr);
            cMsgBox.Show();
        }
    }
    else
    {
        qInfo() << "dbInterface::deleteGymnast(): Db not initialized";
    }
}

void dbInterface::retrieveGymnastSubscriptionList(QList<QStringList>& p_strGymnList)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery queryAthlete(db);
        QSqlQuery querySportEventSub(db);

        const int iEventId = getCurrentEventId();
        querySportEventSub.prepare("SELECT athlete_id, sport_event_id "
                                   "FROM sport_event_subscriptions WHERE sport_event_id = ?");
        querySportEventSub.addBindValue(iEventId);
        querySportEventSub.exec();

        while (querySportEventSub.next())
        {
            const int athleteId = querySportEventSub.value(0).toInt();
            const int eventId = querySportEventSub.value(1).toInt();

            queryAthlete.prepare("SELECT id, first_name, last_name, nation_id "
                                 "FROM athlete WHERE id = ?");
            queryAthlete.addBindValue(athleteId);
            queryAthlete.exec();

            QStringList gymnastSubscript;
            if (queryAthlete.first())
            {

                // SQL quesries with Apostrophe doesn't work, you need to double it
                QString firstName = queryAthlete.value(1).toString().trimmed();
                QString lastName = queryAthlete.value(2).toString().trimmed();

                // add full name
                gymnastSubscript << QString::number(athleteId)
                                 << QString::number(eventId)
                                 << firstName
                                 << lastName
                                 << getNationName(queryAthlete.value(3).toInt(), NI_IocName);
            }
            else
            {
                qCritical() << "No athlete found for Id: " << athleteId;
            }

            p_strGymnList  << gymnastSubscript;
        }
    }
    else
    {
        qInfo() << "dbInterface::getGymnastSelectList(): Db not initialized";
    }
}

bool dbInterface::subscribeGymnasttoEvent(int athleteId, int eventId)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.prepare("INSERT INTO sport_event_subscriptions (athlete_id, sport_event_id) "
                      "VALUES (?, ?)");
        query.addBindValue(athleteId);
        query.addBindValue(eventId);

        const bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "Athlete subscribed: " << athleteId << " to event: " << eventId;
        }
        else
        {
            qCritical() << "Athlete NOT subscribed: " << athleteId << " to event: " << eventId;
            qCritical() << query.lastError();
        }

        return bRet;
    }

    qInfo() << "dbInterface::subscribeGymnasttoEvent(): Db not initialized";
    return false;
}

void dbInterface::deleteGymnastSubsscription(int athleteId, int eventId)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        query.prepare("DELETE FROM sport_event_subscriptions "
                      "WHERE athlete_id = ? AND sport_event_id = ?");
        query.addBindValue(athleteId);
        query.addBindValue(eventId);
        const bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "Athlete unsubribed: " << athleteId << " to event: " << eventId;
        }
        else
        {
            qCritical() << "Athlete NOT unsubscribed: " << athleteId << " to event: " << eventId;
            qCritical() << query.lastError();
        }
    }
    else
    {
        qInfo() << "dbInterface::deleteGymnastSubsscription(): Db not initialized";
    }
}

bool dbInterface::isGymnastAlreadyRegistered(int p_iAthleteId, int p_iEventId)
{
    bool bAthleteFound = false;

    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery querySportEventSub(db);


        querySportEventSub.prepare("SELECT athlete_id, sport_event_id "
                                   "FROM sport_event_subscriptions "
                                   "WHERE athlete_id = ? AND sport_event_id = ?");
        querySportEventSub.addBindValue(p_iAthleteId);
        querySportEventSub.addBindValue(p_iEventId);
        querySportEventSub.exec();

        if (querySportEventSub.first())
        {
            bAthleteFound = true;
        }
    }

    return bAthleteFound;
}

void dbInterface::updateForceScore(const int p_iEventId,
                           const int p_iAthleteId,
                           const int p_iApparatusId,
                           const int p_iForceScore)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);
        // UPDATE table_name
        //SET column1=value1,column2=value2,...
        //WHERE some_column=some_value;
        query.prepare("UPDATE scores SET force_score = ? "
                      "WHERE sport_event_id = ? AND athlete_id = ? AND apparatus_id = ?");
        query.addBindValue(p_iForceScore);
        query.addBindValue(p_iEventId);
        query.addBindValue(p_iAthleteId);
        query.addBindValue(p_iApparatusId);

        bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "Score updated: " << p_iEventId << ", "
                                         << p_iAthleteId << ", "
                                         << p_iApparatusId << ", "
                                         << p_iForceScore;
        }
        else
        {
            qCritical() << "Score NOT updated: " << p_iEventId << ", "
                                                 << p_iAthleteId << ", "
                                                 << p_iApparatusId << ", "
                                                 << p_iForceScore;
            qCritical() << query.lastError();
        }
    }
    else
    {
        qInfo() << "dbInterface::updateScore(): Db not initialized";
    }
}

void dbInterface::eraseAllForcedScore(const int p_iEventId)
{
    if (m_bInitialized)
    {
        QSqlDatabase db = QSqlDatabase::database("ConnMG");
        QSqlQuery query(db);

        query.prepare("UPDATE scores SET force_score = 0 WHERE sport_event_id = ?");
        query.addBindValue(p_iEventId);

        bool bRet = query.exec();

        if (bRet)
        {
            qInfo() << "All Forced Score reset for Event: " << p_iEventId;
        }
        else
        {
            qCritical() << "Score Forced Score reset FAILED";
            qCritical() << query.lastError();
        }
    }
    else
    {
        qInfo() << "dbInterface::updateScore(): Db not initialized";
    }
}
