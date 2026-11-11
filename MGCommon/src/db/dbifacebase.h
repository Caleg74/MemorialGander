#ifndef DBIFACEBASE_H
#define DBIFACEBASE_H

#include "dbconnection.h"
#include <QStringList>

struct AllScores
{
    float DifficultyScore;
    float ExecutionScore;
    float PenaltyScore;
    float FinalScore;
    int   ForceScore;   // to be used only in case of equal score
    bool  IsFinalApparatus;
    float BonusScore;
};

class dbIfaceBase : public dBConnection
{
public:

    enum NationInfo
    {
        NI_Nicename,
        NI_IocName,
        NI_IsoName
    };


    dbIfaceBase();

    [[nodiscard]] int getCurrentEvent() const;
    void setCurrentEvent(int event);
    [[nodiscard]] int getActiveEventYear() const;

    // for cbb
    void getCountriesList(QStringList *p_pList);

    // for cbb (complete gymnast list, not only for this event)
    void getRegisterdGymnastList(QStringList* p_pList);

    // for cbb (gymnast list for this event only)
    void getEventSelectedGymnastList(QStringList* p_pList, bool p_bMen, bool p_bWomen);

    int getGymnastId(QString& p_firstName, QString& p_lastName);

    QString getGender(int p_iAthleteId);

    /** Retrieve global gymnast list, not only the ones registered to this event */
    void retrieveRegisteredGymnastList(QList<QStringList> &p_strGymnList);

    /** Retrieve gymnast list registered to the current event */
    void retrieveGymnastEventList(QList<QStringList>& p_strGymnList);

    int getNationId(QString& p_strNiceName);

    int getApparatusId(QString& p_apparatusName, QString &p_gender);

    QString getNationName(int p_iNationId, enum NationInfo infoType);

    /** returns the eventId for the current event (based on year) */
    int getCurrentEventId();

    bool isScorePresent(const int p_iEventId,
                        const int p_iAthleteId,
                        const int p_iApparatusId);

    void getApparatusList(QStringList* p_pList);

    float getDifficultyScore(const int p_iAthleteId, const int p_iApparatusId);

    float getFinalScore(const int p_iAthleteId, const int p_iApparatusId);

    AllScores getAllScores(const int p_iAthleteId, const int p_iApparatusId);

    int getForceScore(const int p_iAthleteId, const int p_iApparatusId);

    float getAllroundTotalScore(const int p_iAthleteId);

    void retrieveChronologicalList(QList<QStringList>& p_strChronoList);

    /** Gets the latest inserted score.
     * @param[in]  p_strGender String "M" or "F"
     * @param[out] p_pAthleteId Athlete ID
     * @param[out] p_pApparatusId Apparatus ID
     * @return true if a valid entry has been found */
    bool getLatestScore(const QString p_strGender, int* p_pAthleteId, int* p_pApparatusId);

private:
    int m_iCurrentEvent;
};

#endif // DBIFACEBASE_H
