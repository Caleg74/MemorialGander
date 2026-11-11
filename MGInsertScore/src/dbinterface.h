#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include "dbifacebase.h"
#include <QStringList>
#include <optional>

class dbInterface : public dbIfaceBase
{
public:
    struct ExistingScore {
        double difficulty;
        QString penalty;
        QString bonus;
    };

    static dbInterface* Instance();

    int getNationId(QString& p_strNiceName);

    int getFinalApparatusId(int athleteId, int eventId);
    std::optional<ExistingScore> getExistingScore(int eventId, int athleteId, int apparatusId);

    bool setNewScore(const int p_iEventId,
                     const int p_iAthleteId,
                     const int p_iApparatusId,
                     const float p_fDifficultyScore,
                     const float p_fExecutionScore,
                     const float p_fBonusScore,
                     const float p_fPenaltyScore,
                     const float p_fFinalScore,
                     const bool p_bFinalApparatus);

    bool updateScore(const int p_iEventId,
                     const int p_iAthleteId,
                     const int p_iApparatusId,
                     const float p_fDifficultyScore,
                     const float p_fExecutionScore,
                     const float p_fBonusScore,
                     const float p_fPenaltyScore,
                     const float p_fFinalScore,
                     const bool p_bFinalApparatus);

private:

    /** Private constructor */
    dbInterface();

    static dbInterface* sm_pInstance;
};

#endif // DBINTERFACE_H
