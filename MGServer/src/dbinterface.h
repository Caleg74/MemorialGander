#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include "dbifacebase.h"
#include <QStringList>

class dbInterface : public dbIfaceBase
{
public:

    static dbInterface* Instance();

    bool insertGymnast(const QString& p_strFirstName,
                       const QString& p_strLastName,
                       const QString& p_strGender,
                       int p_iNationId);

    void deleteGymnast(QString& p_strFirstName, QString& p_strLastName);

    void retrieveGymnastSubscriptionList(QList<QStringList> &p_strGymnList);

    bool subscribeGymnasttoEvent(int athleteId, int eventId);

    void deleteGymnastSubsscription(int athleteId, int eventId);

    bool isGymnastAlreadyRegistered(int p_iAthleteId, int p_iEventId);

    int getNationId(QString& p_strNiceName);

    void updateForceScore(const int p_iEventId,
                     const int p_iAthleteId,
                     const int p_iApparatusId,
                     const int p_iForceScore);

    void eraseAllForcedScore(const int p_iEventId);

private:

    /** Private constructor */
    dbInterface();

    static dbInterface* sm_pInstance;
};

#endif // DBINTERFACE_H
