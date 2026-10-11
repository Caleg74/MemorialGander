#include "allroundmendatamodel.h"
#include "dbinterface.h"
#include "rankingrules.h"
#include "sortfilterproxymodel.h"

//**** STATIC MEMBER INITIALIZATION *********************

AllroundMenDataModel::AllroundMenDataModel(QObject *parent)
    : GymnastDataModel(parent)
{
    RetrieveGymnastList();
}

AllroundMenDataModel::~AllroundMenDataModel()
{
    m_rankingList.clear();
}

void AllroundMenDataModel::RetrieveGymnastList()
{
    QString firstName;
    QString lastName;
    QString countryIso;
    QString countryIoc;
    QString countryFile;
    int athleteId;

    QList<QStringList> p_strGymnList;
    dbInterface::Instance()->retrieveGymnastEventList(p_strGymnList);

    for (int i = 0; i < p_strGymnList.size();i++)
    {
        if (p_strGymnList.at(i)[3] == "M")  // only Men ranking here
        {
            firstName = p_strGymnList.at(i)[0];
            lastName = p_strGymnList.at(i)[1];
            countryIso = dbInterface::Instance()->getNationName(p_strGymnList.at(i)[2].toInt(), dbIfaceBase::NI_IsoName);
            countryIoc = dbInterface::Instance()->getNationName(p_strGymnList.at(i)[2].toInt(), dbIfaceBase::NI_IocName);
            countryFile = "qrc:/flags/" + countryIso.toLower() + ".svg";
            athleteId = dbInterface::Instance()->getGymnastId(firstName, lastName);

            AthleteData cAllroundMen(AthleteData::Male, athleteId, firstName
                    + " " + lastName + "  ", countryIoc, countryFile);

            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_rankingList << cAllroundMen;
            endInsertRows();
        }
    }
}

void AllroundMenDataModel::updateScores()
{
    QList<AthleteData>::iterator iter;
    int iLatestScoreAthletedId = 0;
    int iLatestScoreApparatusId = 0;

    bool bLatestScoreIsValid = dbInterface::Instance()->getLatestScore("M", &iLatestScoreAthletedId, &iLatestScoreApparatusId);

    for (iter = m_rankingList.begin(); iter != m_rankingList.end(); ++iter)
    {
        for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AMApparatusMax; apparatus++)
        {
            int iAthleteId = iter->getAthleteId();
            int iAppId = ApparatusList::Instance()->getApparatusId((ApparatusList::EApparatusMen)apparatus);
            AllScores sAllScore = dbInterface::Instance()->getAllScores(iAthleteId, iAppId);
//            float fFinalRandom = (float)(qrand() % ((15000 + 1) - 10000) + 10000) / 1000;
//            float fStartRandom = fFinalRandom - 10;
            iter->setFinalScore((ApparatusList::EApparatusMen)apparatus, sAllScore.FinalScore);
            iter->setDifficultyScore((ApparatusList::EApparatusMen)apparatus, sAllScore.DifficultyScore);
            iter->setExecutionScore(apparatus, sAllScore.ExecutionScore);
            iter->setPenaltyScore(apparatus, sAllScore.PenaltyScore);
            iter->setIsFinalApparatusScore((ApparatusList::EApparatusMen)apparatus, sAllScore.IsFinalApparatus);

            bool bLatestScore = (bLatestScoreIsValid && (iLatestScoreAthletedId == iAthleteId) && (iLatestScoreApparatusId == iAppId));
            iter->setIsLatestScore((ApparatusList::EApparatusMen)apparatus, bLatestScore);
        }
        iter->CalculateTotalScore();
    }

    RankingRules::sortAndAssignRanks(m_rankingList);

    // update all model
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
}


//void AllroundMenDataModel::addItem(QString nameFull)
//{

//    int iNationId = dbInterface::Instance()->getNationId(country);

//    AllroundMenData cAllroundMen(nameFull);

//    // add it to the database
//    dbInterface::Instance()->insertGymnast(firstName, lastName, sex, iNationId);

//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//cAllroundMen    endInsertRows();
//}

//void AllroundMenDataModel::removeItem(QString nameFull)
//{
//    AllroundMenData* pItem = GetItem(firstName, lastName);

//    QModelIndex pIndex = indexFromItem(pItem);

//    // add it to the database
//    dbInterface::Instance()->deleteGymnast(firstName, lastName);

//    beginRemoveRows(QModelIndex(), pIndex.row(), pIndex.row());
//    m_rankingList.removeAt(pIndex.row());
//    endRemoveRows();
//}

int AllroundMenDataModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_rankingList.count();
}

QVariant AllroundMenDataModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_rankingList.count())
        return QVariant();

    const AthleteData &gymnast = m_rankingList[index.row()];
    if (role == RankRole)
        return gymnast.getRank();
    else if (role == NameFullRole)
        return gymnast.getNameFull();
    else if (role == FlagImageRole)
        return gymnast.getImagePath();
    else if (role == DifficultyScoreTotalRole)
        return gymnast.getDifficultyScore(ApparatusList::AGTotalScore);
    else if (role == FinalScoreTotalRole)
        return gymnast.getFinalScore(ApparatusList::AGTotalScore);
    else if (role == DifficultyScoreFloorRole)
        return gymnast.getDifficultyScore(ApparatusList::AMFloor);
    else if (role == FinalScoreFloorRole)
        return gymnast.getFinalScore(ApparatusList::AMFloor);
    else if (role == FinalApparatusFloorRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMFloor);
    else if (role == LatestScoreFloorRole)
        return gymnast.isLatestScore(ApparatusList::AMFloor);
    else if (role == DifficultyScorePHorseRole)
        return gymnast.getDifficultyScore(ApparatusList::AMPommelHorse);
    else if (role == FinalScorePHorseRole)
        return gymnast.getFinalScore(ApparatusList::AMPommelHorse);
    else if (role == FinalApparatusPHorseRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMPommelHorse);
    else if (role == LatestScorePHorseRole)
        return gymnast.isLatestScore(ApparatusList::AMPommelHorse);
    else if (role == DifficultyScoreRingsRole)
        return gymnast.getDifficultyScore(ApparatusList::AMRings);
    else if (role == FinalScoreRingsRole)
        return gymnast.getFinalScore(ApparatusList::AMRings);
    else if (role == FinalApparatusRingsRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMRings);
    else if (role == LatestScoreRingsRole)
        return gymnast.isLatestScore(ApparatusList::AMRings);
    else if (role == DifficultyScoreVaultRole)
        return gymnast.getDifficultyScore(ApparatusList::AMVault);
    else if (role == FinalScoreVaultRole)
        return gymnast.getFinalScore(ApparatusList::AMVault);
    else if (role == FinalApparatusVaultRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMVault);
    else if (role == LatestScoreVaultRole)
        return gymnast.isLatestScore(ApparatusList::AMVault);
    else if (role == DifficultyScorePBarsRole)
        return gymnast.getDifficultyScore(ApparatusList::AMParallelBars);
    else if (role == FinalScorePBarsRole)
        return gymnast.getFinalScore(ApparatusList::AMParallelBars);
    else if (role == FinalApparatusPBarsRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMParallelBars);
    else if (role == LatestScorePBarsRole)
        return gymnast.isLatestScore(ApparatusList::AMParallelBars);
    else if (role == DifficultyScoreHBarRole)
        return gymnast.getDifficultyScore(ApparatusList::AMHorizontalBar);
    else if (role == FinalScoreHBarRole)
        return gymnast.getFinalScore(ApparatusList::AMHorizontalBar);
    else if (role == FinalApparatusHBarRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AMHorizontalBar);
    else if (role == LatestScoreHBarRole)
        return gymnast.isLatestScore(ApparatusList::AMHorizontalBar);

    return QVariant();
}

QHash<int, QByteArray> AllroundMenDataModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RankRole            ]      = "Rank";
    roles[NameFullRole        ]      = "NameFull";
    roles[FlagImageRole       ]      = "FlagImage";
    roles[DifficultyScoreTotalRole ]      = "DifficultyScore_Total";
    roles[FinalScoreTotalRole ]      = "FinalScore_Total";
    roles[DifficultyScoreFloorRole ]      = "DifficultyScore_Floor";
    roles[FinalScoreFloorRole ]      = "FinalScore_Floor";
    roles[FinalApparatusFloorRole ]  = "FinalApparatus_Floor";
    roles[LatestScoreFloorRole ]     = "LatestScore_Floor";
    roles[DifficultyScorePHorseRole]      = "DifficultyScore_PHorse";
    roles[FinalScorePHorseRole]      = "FinalScore_PHorse";
    roles[FinalApparatusPHorseRole ] = "FinalApparatus_PHorse";
    roles[LatestScorePHorseRole ]    = "LatestScore_PHorse";
    roles[DifficultyScoreRingsRole ]      = "DifficultyScore_Rings";
    roles[FinalScoreRingsRole ]      = "FinalScore_Rings";
    roles[FinalApparatusRingsRole ]  = "FinalApparatus_Rings";
    roles[LatestScoreRingsRole ]     = "LatestScore_Rings";
    roles[DifficultyScoreVaultRole ]      = "DifficultyScore_Vault";
    roles[FinalScoreVaultRole ]      = "FinalScore_Vault";
    roles[FinalApparatusVaultRole ]  = "FinalApparatus_Vault";
    roles[LatestScoreVaultRole ]     = "LatestScore_Vault";
    roles[DifficultyScorePBarsRole ]      = "DifficultyScore_ParallelBars";
    roles[FinalScorePBarsRole ]      = "FinalScore_ParallelBars";
    roles[FinalApparatusPBarsRole ]  = "FinalApparatus_ParallelBars";
    roles[LatestScorePBarsRole ]     = "LatestScore_ParallelBars";
    roles[DifficultyScoreHBarRole  ]      = "DifficultyScore_HBar";
    roles[FinalScoreHBarRole  ]      = "FinalScore_HBar";
    roles[FinalApparatusHBarRole ]   = "FinalApparatus_HBar";
    roles[LatestScoreHBarRole ]      = "LatestScore_HBar";

    return roles;
}

QModelIndex AllroundMenDataModel::indexFromItem(const AthleteData* item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_rankingList.size(); ++row)
    {
        if(m_rankingList.at(row) == (*item)) return index(row);
    }
    return QModelIndex();
}

AthleteData* AllroundMenDataModel::GetItem(QString& nameFull)
{
    QList<AthleteData>::iterator iter;
    for (iter = m_rankingList.begin(); iter != m_rankingList.end(); ++iter)
    {
        if ((iter)->getNameFull() == nameFull)
            return &(*iter);
    }

    return nullptr;
}

bool AllroundMenDataModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    Q_UNUSED(source_row);
    Q_UNUSED(source_parent);

    return true;
}
