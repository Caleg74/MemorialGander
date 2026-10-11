#include "allroundwomendatamodel.h"
#include "dbinterface.h"
#include "rankingrules.h"
#include "sortfilterproxymodel.h"

//**** STATIC MEMBER INITIALIZATION *********************

AllroundWomenDataModel::AllroundWomenDataModel(QObject *parent)
    : GymnastDataModel(parent)
{
    RetrieveGymnastList();

}

AllroundWomenDataModel::~AllroundWomenDataModel()
{
    m_rankingList.clear();
}

void AllroundWomenDataModel::RetrieveGymnastList()
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
        if (p_strGymnList.at(i)[3] == "F")  // only Women ranking here
        {
            firstName = p_strGymnList.at(i)[0];
            lastName = p_strGymnList.at(i)[1];
            countryIso = dbInterface::Instance()->getNationName(p_strGymnList.at(i)[2].toInt(), dbIfaceBase::NI_IsoName);
            countryIoc = dbInterface::Instance()->getNationName(p_strGymnList.at(i)[2].toInt(), dbIfaceBase::NI_IocName);
            countryFile = "qrc:/flags/" + countryIso.toLower() + ".svg";
            athleteId = dbInterface::Instance()->getGymnastId(firstName, lastName);
            AthleteData cAllroundWomen(AthleteData::Female, athleteId, firstName
                    + " " + lastName + "  ", countryIoc, countryFile);

            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_rankingList << cAllroundWomen;
            endInsertRows();
        }
    }
}

void AllroundWomenDataModel::updateScores()
{
    QList<AthleteData>::iterator iter;
    int iLatestScoreAthletedId = 0;
    int iLatestScoreApparatusId = 0;

    bool bLatestScoreIsValid = dbInterface::Instance()->getLatestScore("F", &iLatestScoreAthletedId, &iLatestScoreApparatusId);

    for (iter = m_rankingList.begin(); iter != m_rankingList.end(); ++iter)
    {
        for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AWApparatusMax; apparatus++)
        {
            int iAthleteId = iter->getAthleteId();
            int iAppId = ApparatusList::Instance()->getApparatusId((ApparatusList::EApparatusWomen)apparatus);
            AllScores sAllScore = dbInterface::Instance()->getAllScores(iAthleteId, iAppId);

//            float fFinalRandom = (float)(qrand() % ((15000 + 1) - 10000) + 10000) / 1000;
//            float fStartRandom = fFinalRandom - 10;
            iter->setFinalScore((ApparatusList::EApparatusWomen)apparatus, sAllScore.FinalScore);
            iter->setDifficultyScore((ApparatusList::EApparatusWomen)apparatus, sAllScore.DifficultyScore);
            iter->setExecutionScore(apparatus, sAllScore.ExecutionScore);
            iter->setPenaltyScore(apparatus, sAllScore.PenaltyScore);
            iter->setIsFinalApparatusScore((ApparatusList::EApparatusWomen)apparatus, sAllScore.IsFinalApparatus);

            bool bLatestScore = (bLatestScoreIsValid && (iLatestScoreAthletedId == iAthleteId) && (iLatestScoreApparatusId == iAppId));
            iter->setIsLatestScore((ApparatusList::EApparatusWomen)apparatus, bLatestScore);
        }
        iter->CalculateTotalScore();
    }

    RankingRules::sortAndAssignRanks(m_rankingList);

    // update all model
    emit dataChanged(index(0, 0), index(rowCount() - 1, 0));

}


//void AllroundWomenDataModel::addItem(QString nameFull)
//{

//    int iNationId = dbInterface::Instance()->getNationId(country);

//    AllroundWomenData cAllroundWomen(nameFull);

//    // add it to the database
//    dbInterface::Instance()->insertGymnast(firstName, lastName, sex, iNationId);

//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//cAllroundWomen    endInsertRows();
//}

//void AllroundWomenDataModel::removeItem(QString nameFull)
//{
//    AllroundWomenData* pItem = GetItem(firstName, lastName);

//    QModelIndex pIndex = indexFromItem(pItem);

//    // add it to the database
//    dbInterface::Instance()->deleteGymnast(firstName, lastName);

//    beginRemoveRows(QModelIndex(), pIndex.row(), pIndex.row());
//    m_rankingList.removeAt(pIndex.row());
//    endRemoveRows();
//}

int AllroundWomenDataModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_rankingList.count();
}

QVariant AllroundWomenDataModel::data(const QModelIndex & index, int role) const {
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
    else if (role == DifficultyScoreVaultRole)
        return gymnast.getDifficultyScore(ApparatusList::AWVault);
    else if (role == FinalScoreVaultRole)
        return gymnast.getFinalScore(ApparatusList::AWVault);
    else if (role == FinalApparatusVaultRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AWVault);
    else if (role == LatestScoreVaultRole)
        return gymnast.isLatestScore(ApparatusList::AWVault);
    else if (role == DifficultyScoreUnBarsRole)
        return gymnast.getDifficultyScore(ApparatusList::AWUnevenBars);
    else if (role == FinalScoreUnBarsRole)
        return gymnast.getFinalScore(ApparatusList::AWUnevenBars);
    else if (role == FinalApparatusUnBarsRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AWUnevenBars);
    else if (role == LatestScoreUnBarsRole)
        return gymnast.isLatestScore(ApparatusList::AWUnevenBars);
    else if (role == DifficultyScoreBalBeamRole)
        return gymnast.getDifficultyScore(ApparatusList::AWBalanceBeam);
    else if (role == FinalScoreBalBeamRole)
        return gymnast.getFinalScore(ApparatusList::AWBalanceBeam);
    else if (role == FinalApparatusBalBeamRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AWBalanceBeam);
    else if (role == LatestScoreBalBeamRole)
        return gymnast.isLatestScore(ApparatusList::AWBalanceBeam);
    else if (role == DifficultyScoreFloorRole)
        return gymnast.getDifficultyScore(ApparatusList::AWFloor);
    else if (role == FinalScoreFloorRole)
        return gymnast.getFinalScore(ApparatusList::AWFloor);
    else if (role == FinalApparatusFloorRole)
        return gymnast.isFinalApparatusScore(ApparatusList::AWFloor);
    else if (role == LatestScoreFloorRole)
        return gymnast.isLatestScore(ApparatusList::AWFloor);

    return QVariant();
}

QHash<int, QByteArray> AllroundWomenDataModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RankRole             ]     = "Rank";
    roles[NameFullRole         ]     = "NameFull";
    roles[FlagImageRole        ]     = "FlagImage";
    roles[DifficultyScoreTotalRole  ]     = "DifficultyScore_Total";
    roles[FinalScoreTotalRole  ]     = "FinalScore_Total";
    roles[DifficultyScoreFloorRole  ]     = "DifficultyScore_Floor";
    roles[FinalScoreFloorRole  ]     = "FinalScore_Floor";
    roles[FinalApparatusFloorRole]   = "FinalApparatus_Floor";
    roles[LatestScoreFloorRole ]     = "LatestScore_Floor";
    roles[DifficultyScoreUnBarsRole ]     = "DifficultyScore_UnBars";
    roles[FinalScoreUnBarsRole ]     = "FinalScore_UnBars";
    roles[FinalApparatusUnBarsRole]  = "FinalApparatus_UnBars";
    roles[LatestScoreUnBarsRole ]    = "LatestScore_UnBars";
    roles[DifficultyScoreBalBeamRole]     = "DifficultyScore_BalBeam";
    roles[FinalScoreBalBeamRole]     = "FinalScore_BalBeam";
    roles[FinalApparatusBalBeamRole] = "FinalApparatus_BalBeam";
    roles[LatestScoreBalBeamRole ]   = "LatestScore_BalBeam";
    roles[DifficultyScoreVaultRole  ]     = "DifficultyScore_Vault";
    roles[FinalScoreVaultRole  ]     = "FinalScore_Vault";
    roles[FinalApparatusVaultRole]   = "FinalApparatus_Vault";
    roles[LatestScoreVaultRole ]     = "LatestScore_Vault";


    return roles;
}

QModelIndex AllroundWomenDataModel::indexFromItem(const AthleteData* item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_rankingList.size(); ++row)
    {
        if(m_rankingList.at(row) == (*item)) return index(row);
    }
    return QModelIndex();
}

AthleteData *AllroundWomenDataModel::GetItem(QString& nameFull)
{
    QList<AthleteData>::iterator iter;
    for (iter = m_rankingList.begin(); iter != m_rankingList.end(); ++iter)
    {
        if ((iter)->getNameFull() == nameFull)
            return &(*iter);
    }

    return nullptr;
}
