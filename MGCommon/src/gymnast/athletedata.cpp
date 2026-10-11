/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "athletedata.h"
#include <QDebug>
#include <algorithm>
#include <functional>
#include "rankingrules.h"
#include "arithmetictools.h"

AthleteData::AthleteData(const Gender_t p_eGender,
                         const int p_iAthleteId,
                         const QString& fullName,
                         const QString& nation,
                         const QString& imagePath)
    : m_eGender(p_eGender)
    , m_iAthleteId(p_iAthleteId)
    , m_iRank(0)
    , m_nameFull(fullName)
    , m_nationShort(nation)
    , m_imagePath(imagePath)
{
    int iApparatusMax;
    if (m_eGender == Male)
        iApparatusMax = ApparatusList::AMApparatusMax;
    else
        iApparatusMax = ApparatusList::AWApparatusMax;

    for (int i=0; i<iApparatusMax; i++)
    {
        SScore score = {0.0, 0.0, 0.0, 0.0, false, false};
        m_vecScore.push_back(score);
    }
}

int AthleteData::getAthleteId()
{
    return m_iAthleteId;
}

void AthleteData::setRank(int p_iRank)
{
    m_iRank = p_iRank;
}

QString AthleteData::getRank() const
{
    return QString::number(m_iRank, 10);
}

QString AthleteData::getNameFull() const
{
    return m_nameFull;
}

QString AthleteData::getNationShort() const
{
    return m_nationShort;
}
QString AthleteData::getImagePath() const
{
    return m_imagePath;
}

void AthleteData::setIsFinalApparatusScore(int p_iApparatus, bool p_bIsFinalApparatus)
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    m_vecScore[p_iApparatus].IsFinalApparatus = p_bIsFinalApparatus;
}

bool AthleteData::isFinalApparatusScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    return m_vecScore[p_iApparatus].IsFinalApparatus;
}

void AthleteData::setIsLatestScore(int p_iApparatus, bool p_bIsLatestScore)
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    m_vecScore[p_iApparatus].IsLatestScore = p_bIsLatestScore;
}

bool AthleteData::isLatestScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    return m_vecScore[p_iApparatus].IsLatestScore;
}

void AthleteData::setDifficultyScore(int p_iApparatus, float p_fScore)
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "setDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    m_vecScore[p_iApparatus].DifficultyScore = p_fScore;
}

QString AthleteData::getDifficultyScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    if (fAreSame(m_vecScore[p_iApparatus].DifficultyScore, 0.0f))
        return " - ";
    else
        return QString::number(m_vecScore[p_iApparatus].DifficultyScore, 'f', 3);
}

void AthleteData::setExecutionScore(int p_iApparatus, float p_fScore)
{
    if (p_iApparatus < 0 || p_iApparatus >= m_vecScore.size()) {
        qCritical() << "setExecutionScore(): apparatus out of range" << p_iApparatus;
        return;
    }
    m_vecScore[p_iApparatus].ExecutionScore = p_fScore;
}

QString AthleteData::getExecutionScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    if (fAreSame(m_vecScore[p_iApparatus].ExecutionScore, 0.0f))
        return " - ";
    else
        return QString::number(m_vecScore[p_iApparatus].ExecutionScore, 'f', 3);
}

void AthleteData::setPenaltyScore(int p_iApparatus, float p_fScore)
{
    if (p_iApparatus < 0 || p_iApparatus >= m_vecScore.size()) {
        qCritical() << "setPenaltyScore(): apparatus out of range" << p_iApparatus;
        return;
    }
    m_vecScore[p_iApparatus].PenaltyScore = p_fScore;
}

QString AthleteData::getPenaltyScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getDifficultyScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    if (fAreSame(m_vecScore[p_iApparatus].PenaltyScore, 0.0f))
        return " - ";
    else
        return QString::number(m_vecScore[p_iApparatus].PenaltyScore, 'f', 3);
}

void AthleteData::setFinalScore(int p_iApparatus, float p_fScore)
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "setFinalScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "setFinalScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    m_vecScore[p_iApparatus].FinalScore = p_fScore;
}

QString AthleteData::getFinalScore(int p_iApparatus) const
{
    if (m_eGender == Male)
    {
        if (p_iApparatus >= (ApparatusList::AMApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(M) " << p_iApparatus << " is out-of-range";
    }
    else
    {
        if (p_iApparatus >= (ApparatusList::AWApparatusMax))
            qCritical() << "getFinalScore(): p_iApparatus(W) " << p_iApparatus << " is out-of-range";
    }

    if (fAreSame(m_vecScore[p_iApparatus].FinalScore, 0.0f))
        return " - ";
    else
        return QString::number(m_vecScore[p_iApparatus].FinalScore, 'f', 3);
}

void AthleteData::CalculateTotalScore()
{
    qint64 difficulty = 0;
    qint64 execution = 0;
    qint64 penalty = 0;
    qint64 final = 0;
    for (qsizetype i = ApparatusList::AGFirstApparatus; i < m_vecScore.size(); ++i) {
        const auto &score = m_vecScore.at(i);
        difficulty += RankingRules::milliPoints(score.DifficultyScore);
        execution += RankingRules::milliPoints(score.ExecutionScore);
        penalty += RankingRules::milliPoints(score.PenaltyScore);
        final += RankingRules::milliPoints(score.FinalScore);
    }
    auto &total = m_vecScore[ApparatusList::AGTotalScore];
    total.DifficultyScore = static_cast<float>(difficulty / 1000.0);
    total.ExecutionScore = static_cast<float>(execution / 1000.0);
    total.PenaltyScore = static_cast<float>(penalty / 1000.0);
    total.FinalScore = static_cast<float>(final / 1000.0);
}

QList<qint64> AthleteData::rankingKey() const
{
    QList<qint64> finals;
    qint64 total = 0;
    qint64 execution = 0;
    qint64 difficulty = 0;
    for (qsizetype i = ApparatusList::AGFirstApparatus; i < m_vecScore.size(); ++i) {
        const auto &score = m_vecScore.at(i);
        const qint64 final = RankingRules::milliPoints(score.FinalScore);
        finals.append(final);
        total += final;
        execution += RankingRules::milliPoints(score.ExecutionScore);
        difficulty += RankingRules::milliPoints(score.DifficultyScore);
    }
    std::sort(finals.begin(), finals.end(), std::greater<qint64>());

    // FIG overall: total, best N-1 scores, then N-2 ... 1, total E, total D.
    // Missing apparatus retain zero scores, as in the existing Memorial format.
    QList<qint64> key{total};
    for (qsizetype remaining = finals.size(); remaining > 1; --remaining) {
        total -= finals.at(remaining - 1);
        key.append(total);
    }
    key.append(execution);
    key.append(difficulty);
    return key;
}

bool AthleteData::operator<(const AthleteData &other) const
{
    const auto key = rankingKey();
    const auto otherKey = other.rankingKey();
    return std::lexicographical_compare(key.cbegin(), key.cend(),
                                        otherKey.cbegin(), otherKey.cend(),
                                        std::greater<qint64>());
}

bool AthleteData::hasSameRankingAs(const AthleteData &other) const
{
    return rankingKey() == other.rankingKey();
}

bool operator== (const AthleteData& lhs, const AthleteData& rhs)
{
    if (lhs.getNameFull() == rhs.getNameFull())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QDebug &operator<<(QDebug &stream, const AthleteData &obj)
{
    stream << "{" << obj.m_nameFull << ", ";
    stream << obj.m_iRank << ", ";
    stream << obj.m_vecScore[ApparatusList::AGTotalScore].FinalScore << "}\n";

    return stream;
}
