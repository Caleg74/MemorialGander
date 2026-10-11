#ifndef RANKINGRULES_H
#define RANKINGRULES_H

#include <QtGlobal>
#include <algorithm>

namespace RankingRules {

// Compare scores at the same three-decimal precision used by the rankings.
// Integer sums avoid floating-point noise deciding a sporting tie.
inline qint64 milliPoints(float score)
{
    return qRound64(static_cast<double>(score) * 1000.0);
}

template<typename RankingList>
void sortAndAssignRanks(RankingList &gymnasts)
{
    // Sporting ties retain their display order, without an alphabetical tiebreak.
    std::stable_sort(gymnasts.begin(), gymnasts.end());
    int rank = 1;
    for (qsizetype i = 0; i < gymnasts.size(); ++i) {
        if (i > 0 && !gymnasts.at(i).hasSameRankingAs(gymnasts.at(i - 1)))
            rank = static_cast<int>(i + 1);
        gymnasts[i].setRank(rank); // Competition ranking: 1, 1, 3.
    }
}

} // namespace RankingRules

#endif // RANKINGRULES_H
