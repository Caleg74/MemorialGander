#ifndef ATHLETEDATA_H
#define ATHLETEDATA_H

#include <QString>
#include <QList>
#include "apparatuslist.h"


class AthleteData
{
public:

    enum Gender_t
    {
        Male,
        Female
    };

    struct SScore
    {
        float DifficultyScore;
        float ExecutionScore;
        float PenaltyScore;
        float FinalScore;
        bool  IsFinalApparatus; // mark if score id fromthe final apparatus
        bool  IsLatestScore;    // true if last-inserted score
    };

    AthleteData(const Gender_t p_eGender,
                const int p_iAthleteId,
                const QString& fullName,
                const QString& nation,
                const QString& imagePath);

    int getAthleteId();
    void setRank(int p_iRank);
    QString getRank() const;
    QString getNameFull() const;
    QString getNationShort() const;
    QString getImagePath() const;

    void setIsFinalApparatusScore(int p_iApparatus, bool p_bIsFinalApparatus);
    bool isFinalApparatusScore(int p_iApparatus) const;

    void setIsLatestScore(int p_iApparatus, bool p_bIsLatestScore);
    bool isLatestScore(int p_iApparatus) const;

    void setDifficultyScore(int p_iApparatus, float p_fScore);
    QString getDifficultyScore(int p_iApparatus) const;

    void setExecutionScore(int p_iApparatus, float p_fScore);
    QString getExecutionScore(int p_iApparatus) const;

    void setPenaltyScore(int p_iApparatus, float p_fScore);
    QString getPenaltyScore(int p_iApparatus) const;

    void setFinalScore(int p_iApparatus, float p_fScore);
    QString getFinalScore(int p_iApparatus) const;

    void CalculateTotalScore();

    friend bool operator== (const AthleteData& lhs, const AthleteData& rhs);

    friend QDebug& operator<< (QDebug &stream, const AthleteData &obj);

    bool operator<(const AthleteData &other) const;
    bool hasSameRankingAs(const AthleteData &other) const;



private:
    QList<qint64> rankingKey() const;

    Gender_t m_eGender;
    int m_iAthleteId;
    int m_iRank;
    QString m_nameFull;
    QString m_nationShort;
    QString m_imagePath;
    QList<SScore> m_vecScore;
};



#endif // ATHLETEDATA_H
