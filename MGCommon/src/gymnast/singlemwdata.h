#ifndef SINGLEMWDATA_H
#define SINGLEMWDATA_H

#include <QString>
#include "apparatuslist.h"

class SingleMWData
{
public:


    struct SScore
    {
        float DifficultyScore;
        float ExecutionScore;
        float PenaltyScore;
        float FinalScore;
        int   ForceScore;   // Legacy metadata; not used for sporting tiebreaks
    };

    SingleMWData(const int p_iAthleteId,
                 const QString& fullName,
                 const QString& nation,
                 const QString& imagePath);

    int getAthleteId();
    void setRank(int p_iRank);
    QString getRank() const;
    QString getNameFull() const;
    QString getNation() const;
    QString getImagePath() const;

    void setDifficultyScore(float p_fScore);
    QString getDifficultyScore() const;

    void setExecutionScore(float p_fScore);
    QString getExecutionScore() const;

    void setPenaltyScore(float p_fScore);
    QString getPenaltyScore() const;

    void setFinalScore(float p_fScore);
    QString getFinalScore() const;

    void setForceScore(int p_iForceScore);

    // Legacy metadata; apparatus tiebreaks use only final, E and D.
    void setAllroundTotalScore(float p_fAllroundTotScore);

//    QString getForceScore() const;

    friend bool operator== (const SingleMWData& lhs, const SingleMWData& rhs);

    friend QDebug& operator<< (QDebug &stream, const SingleMWData &obj);

    bool operator<(const SingleMWData &other) const;
    bool hasSameRankingAs(const SingleMWData &other) const;


private:
    int m_iAthleteId;
    int m_iRank;
    float m_fAllroundScore;    // Legacy metadata, not a ranking criterion
    QString m_nameFull;
    QString m_nation;
    QString m_imagePath;
    SScore m_sScore{};
};



#endif // SINGLEMWDATA_H
