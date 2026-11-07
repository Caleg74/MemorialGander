#include <QtTest>
#include <cmath>
#include "athletedata.h"
#include "singlemwdata.h"
#include "rankingrules.h"

class RankingTest : public QObject
{
    Q_OBJECT

    static SingleMWData single(const QString &name, float d, float e, float penalty)
    {
        SingleMWData gymnast(1, name, "SUI", "");
        gymnast.setDifficultyScore(d);
        gymnast.setExecutionScore(e);
        gymnast.setPenaltyScore(-penalty);
        gymnast.setFinalScore(d + e - penalty);
        return gymnast;
    }

    static AthleteData overall(AthleteData::Gender_t gender, const QString &name,
                               float d, float e, float penalty)
    {
        AthleteData gymnast(gender, 1, name, "SUI", "");
        const int end = gender == AthleteData::Male ? static_cast<int>(ApparatusList::AMApparatusMax)
                                                   : static_cast<int>(ApparatusList::AWApparatusMax);
        for (int app = ApparatusList::AGFirstApparatus; app < end; ++app) {
            gymnast.setDifficultyScore(app, d);
            gymnast.setExecutionScore(app, e);
            gymnast.setPenaltyScore(app, -penalty);
            gymnast.setFinalScore(app, d + e - penalty);
        }
        gymnast.CalculateTotalScore();
        return gymnast;
    }

    static AthleteData withFinals(AthleteData::Gender_t gender, const QString &name,
                                 const QList<float> &finals)
    {
        AthleteData gymnast(gender, 1, name, "SUI", "");
        for (qsizetype i = 0; i < finals.size(); ++i) {
            const int app = static_cast<int>(i + ApparatusList::AGFirstApparatus);
            gymnast.setDifficultyScore(app, finals.at(i) - 9.0f);
            gymnast.setExecutionScore(app, 9.0f);
            gymnast.setPenaltyScore(app, 0.0f);
            gymnast.setFinalScore(app, finals.at(i));
        }
        gymnast.CalculateTotalScore();
        return gymnast;
    }

private slots:
    void singleUsesRealExecution()
    {
        const auto a = single("A", 7.5f, 9.2f, 0.3f);
        const auto b = single("B", 7.4f, 9.0f, 0.0f);
        QCOMPARE(a.getFinalScore(), b.getFinalScore());
        QVERIFY(a < b);
        QVERIFY(!(b < a));
    }

    void singleDifficultyAfterEqualExecution()
    {
        const auto a = single("A", 7.5f, 9.0f, 0.2f);
        const auto b = single("B", 7.3f, 9.0f, 0.0f);
        QCOMPARE(a.getFinalScore(), b.getFinalScore());
        QCOMPARE(a.getExecutionScore(), b.getExecutionScore());
        QVERIFY(a < b);
        QVERIFY(!(b < a));
    }

    void singleFinalHasPriority()
    {
        const auto a = single("A", 8.5f, 8.0f, 0.0f);
        const auto b = single("B", 7.0f, 9.0f, 0.0f);
        QVERIFY(a < b);
    }

    void singleExactTieIgnoresLegacyCriteria()
    {
        auto a = single("Z", 7.5f, 9.0f, 0.1f);
        auto b = single("A", 7.5f, 9.0f, 0.1f);
        a.setAllroundTotalScore(0);
        b.setAllroundTotalScore(100);
        a.setForceScore(0);
        b.setForceScore(99);
        QVERIFY(a.hasSameRankingAs(b));
        QVERIFY(!(a < b));
        QVERIFY(!(b < a));
        QList<SingleMWData> gymnasts{a, b, single("C", 7.0f, 9.0f, 0.0f)};
        RankingRules::sortAndAssignRanks(gymnasts);
        QCOMPARE(gymnasts.at(0).getNameFull(), QString("Z"));
        QCOMPARE(gymnasts.at(0).getRank(), QString("1"));
        QCOMPARE(gymnasts.at(1).getRank(), QString("1"));
        QCOMPARE(gymnasts.at(2).getRank(), QString("3"));
    }

    void singleRoundoffDoesNotBreakTies()
    {
        const auto a = single("A", 7.5f, 9.0f, 0.1f);
        auto b = a;
        b.setFinalScore(16.400003f);
        QVERIFY(a.hasSameRankingAs(b));
        b.setFinalScore(16.401f);
        QVERIFY(b < a);
        QVERIFY(!a.hasSameRankingAs(b));
    }

    void zeroSingleScoresAreInitialized()
    {
        SingleMWData a(1, "A", "SUI", "");
        SingleMWData b(2, "B", "SUI", "");
        QCOMPARE(a.getExecutionScore(), QString("0.000"));
        QCOMPARE(a.getPenaltyScore(), QString("0.000"));
        QVERIFY(a.hasSameRankingAs(b));
    }

    void previousSofiaAnnySingleVaultTie()
    {
        const auto sofia = single("Sofia", 7.1f, 9.5f, 0.15f);
        const auto anny = single("Anny", 7.45f, 9.3f, 0.3f);
        QCOMPARE(sofia.getFinalScore(), QString("16.450"));
        QCOMPARE(sofia.getFinalScore(), anny.getFinalScore());
        QVERIFY(sofia < anny);
        QVERIFY(!(anny < sofia));
    }

    void overallTiebreaks_data()
    {
        QTest::addColumn<int>("gender");
        QTest::newRow("men") << int(AthleteData::Male);
        QTest::newRow("women") << int(AthleteData::Female);
    }

    void overallTiebreaks()
    {
        QFETCH(int, gender);
        const auto sex = static_cast<AthleteData::Gender_t>(gender);
        const auto a = overall(sex, "A", 7.5f, 9.2f, 0.3f);
        const auto b = overall(sex, "B", 7.4f, 9.0f, 0);
        QCOMPARE(a.getFinalScore(ApparatusList::AGTotalScore),
                 b.getFinalScore(ApparatusList::AGTotalScore));
        QVERIFY(a < b); // Equal final scores and best-score sums: actual E wins.
        QVERIFY(!(b < a));

        const auto c = overall(sex, "C", 7.5f, 9.0f, 0.2f);
        const auto d = overall(sex, "D", 7.3f, 9.0f, 0);
        QCOMPARE(c.getFinalScore(ApparatusList::AGTotalScore),
                 d.getFinalScore(ApparatusList::AGTotalScore));
        QVERIFY(c < d); // Equal E: D wins, in both comparison directions.
        QVERIFY(!(d < c));
    }

    void overallTotalHasPriority()
    {
        const auto a = withFinals(AthleteData::Female, "A", {17, 17, 17, 17});
        const auto b = withFinals(AthleteData::Female, "B", {18, 18, 18, 13});
        QVERIFY(a < b); // 68 > 67, even though B has the better top three.
    }

    void overallBestScoresHavePriority()
    {
        const auto a = withFinals(AthleteData::Female, "A", {17, 17, 16, 16});
        const auto b = withFinals(AthleteData::Female, "B", {17.5f, 16.5f, 16.5f, 15.5f});
        QCOMPARE(a.getFinalScore(0), b.getFinalScore(0));
        QVERIFY(b < a); // Best three: 50.500 > 50.000.
        const auto c = withFinals(AthleteData::Female, "C", {18, 16, 16, 16});
        QVERIFY(c < a); // Best three and two tie; best one decides.
    }

    void previousJoshuaYulTie()
    {
        const auto j = withFinals(AthleteData::Male, "Joshua", {17.475f, 17.7f, 16.075f, 15.975f});
        const auto y = withFinals(AthleteData::Male, "Yul", {16.7f, 16.625f, 16.775f, 0, 17.125f});
        QCOMPARE(j.getFinalScore(0), QString("67.225"));
        QCOMPARE(j.getFinalScore(0), y.getFinalScore(0));
        QVERIFY(j < y);
    }

    void overallExactTiesShareRank()
    {
        const auto a = withFinals(AthleteData::Female, "Z", {17, 17, 16, 16});
        const auto b = withFinals(AthleteData::Female, "A", {16, 17, 17, 16});
        const auto c = withFinals(AthleteData::Female, "C", {16, 16, 16, 16});
        QVERIFY(a.hasSameRankingAs(b));
        QList<AthleteData> gymnasts{c, a, b};
        RankingRules::sortAndAssignRanks(gymnasts);
        QCOMPARE(gymnasts.at(0).getNameFull(), QString("Z"));
        QCOMPARE(gymnasts.at(0).getRank(), QString("1"));
        QCOMPARE(gymnasts.at(1).getRank(), QString("1"));
        QCOMPARE(gymnasts.at(2).getRank(), QString("3"));
        RankingRules::sortAndAssignRanks(gymnasts);
        QCOMPARE(gymnasts.at(0).getNameFull(), QString("Z"));
    }

    void totalsIncludeActualExecutionAndPenalty()
    {
        auto gymnast = overall(AthleteData::Female, "A", 7.5f, 9.2f, 0.3f);
        QCOMPARE(gymnast.getFinalScore(0), QString("65.600"));
        QCOMPARE(gymnast.getExecutionScore(0), QString("36.800"));
        QCOMPARE(gymnast.getPenaltyScore(0), QString("-1.200"));
        gymnast.CalculateTotalScore();
        QCOMPARE(gymnast.getFinalScore(0), QString("65.600"));
    }

    void emptyLists()
    {
        QList<AthleteData> overall;
        QList<SingleMWData> single;
        RankingRules::sortAndAssignRanks(overall);
        RankingRules::sortAndAssignRanks(single);
        QVERIFY(overall.isEmpty());
        QVERIFY(single.isEmpty());
    }
};

QTEST_GUILESS_MAIN(RankingTest)
#include "tst_ranking.moc"
