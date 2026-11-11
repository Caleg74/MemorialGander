#ifndef CHRONOLISTDATA_H
#define CHRONOLISTDATA_H

#include "athletedata.h"
#include <QString>

class ChronoListData
{
public:
    ChronoListData();

    ~ChronoListData();

    void setGender(const QString& p_strGender);
    QString getGender() const;

    void setApparatusName(const QString& p_strApparatus);
    QString getApparatusName() const;

    void setNameFull(const QString& p_strFullName);
    QString getNameFull() const;

    void setImagePath(const QString& p_strImagePath);
    QString getImagePath() const;

    void setNationShort(const QString& p_strNationShort);
    QString getNationShort() const;

    void setDifficultyScore(const float p_fDifficultyScore);
    QString getDifficultyScore() const;

    void setExecutionScore(const float p_fExecutionScore);
    QString getExecutionScore() const;

    void setBonusScore(float score);
    QString getBonusScore() const;

    void setPenaltyScore(const float p_fPenaltyScore);
    QString getPenaltyScore() const;

    void setFinalScore(const float p_fFinalScore);
    QString getFinalScore() const;

    void setTotScore(const float p_fFinalScore);
    QString getTotScore() const;

    void setRank(const int p_iRank);
    float getRank() const;

    void setIsLatestScore(bool p_bIsLatestScore);
    bool isLatestScore() const;

    friend bool operator== (const ChronoListData& lhs, const ChronoListData& rhs);

private:
    QString m_strGender;
    QString m_strApparatus;
    QString m_strNameFull;
    QString m_strImagePath;
    QString m_strNationShort;
    AthleteData::SScore m_sScore;
    float m_fTotScore;
    int m_iRank;
    bool m_bLatestScore;
};

#endif // CHRONOLISTDATA_H
