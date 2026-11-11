#include "chronolistdatamodel.h"
#include <QDebug>
#include "dbinterface.h"

//**** STATIC MEMBER INITIALIZATION *********************

ChronoListDataModel::ChronoListDataModel(QObject *parent)
    : GymnastDataModel(parent)
{
    FillChronoList();
}

ChronoListDataModel::~ChronoListDataModel()
{
    m_chronoList.clear();
}

void ChronoListDataModel::FillChronoList()
{
    const int ciTableSize = 10;

    for (int i = 0; i < ciTableSize; i++)
    {
        ChronoListData cChronoItem;

        cChronoItem.setGender("-");
        cChronoItem.setNameFull("- -");
        cChronoItem.setNationShort("-");
        cChronoItem.setApparatusName("-");
        cChronoItem.setDifficultyScore(0.0f);
        cChronoItem.setExecutionScore(0.0f);
        cChronoItem.setPenaltyScore(0.0f);
        cChronoItem.setFinalScore(0.0f);
        cChronoItem.setIsLatestScore(false);
//        cChronoItem.setRank(0);

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_chronoList << cChronoItem;
        endInsertRows();
    }
}

void ChronoListDataModel::updateScores()
{
    QList<ChronoListData>::iterator iter;
    QList<QStringList> dbRawList;
    QString countryIoc;
    QString countryIso;
    QString countryFile;

    dbInterface::Instance()->retrieveChronologicalList(dbRawList);

    int iObjIndex=0;
    for (iter = m_chronoList.begin(); iter != m_chronoList.end(); ++iter)
    {
        if (iObjIndex < dbRawList.length())
        {
            iter->setGender(dbRawList.at(iObjIndex).at(1));
            iter->setNameFull(dbRawList.at(iObjIndex).at(2));

            // Set the flag
            countryIso = dbInterface::Instance()->getNationName(dbRawList.at(iObjIndex).at(3).toInt(), dbIfaceBase::NI_IsoName);
            countryFile = "qrc:/flags/" + countryIso.toLower() + ".svg";
            iter->setImagePath(countryFile);

            // set the Nation Short name
            countryIoc = dbInterface::Instance()->getNationName(dbRawList.at(iObjIndex).at(3).toInt(), dbIfaceBase::NI_IocName);
            iter->setNationShort(countryIoc);

            iter->setApparatusName(dbRawList.at(iObjIndex).at(4));
            iter->setDifficultyScore(dbRawList.at(iObjIndex).at(5).toFloat());
            iter->setExecutionScore(dbRawList.at(iObjIndex).at(6).toFloat());
            iter->setBonusScore(dbRawList.at(iObjIndex).at(7).toFloat());
            iter->setPenaltyScore(dbRawList.at(iObjIndex).at(8).toFloat());
            iter->setFinalScore(dbRawList.at(iObjIndex).at(9).toFloat());
            int iAthleteId = dbRawList.at(iObjIndex).at(0).toInt();
            iter->setTotScore(dbInterface::Instance()->getAllroundTotalScore(iAthleteId));
            iter->setIsLatestScore(iObjIndex == 0); // highlight only 1st row
        }
        // next object in the List
        iObjIndex++;
    }

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

int ChronoListDataModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_chronoList.count();
}

QVariant ChronoListDataModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_chronoList.count())
        return QVariant();

    const ChronoListData& chronoItem = m_chronoList[index.row()];

    if (role == MenWomenRole)
        return chronoItem.getGender();
    else if (role == NameFullRole)
        return chronoItem.getNameFull();
    else if (role == FlagImageRole)
        return chronoItem.getImagePath();
    else if (role == CountryRole)
        return chronoItem.getNationShort();
    else if (role == ApparatusRole)
        return chronoItem.getApparatusName();
    else if (role == DifficultyScoreRole)
        return chronoItem.getDifficultyScore();
    else if (role == ExecutionScoreRole)
        return chronoItem.getExecutionScore();
    else if (role == BonusScoreRole)
        return chronoItem.getBonusScore();
    else if (role == PenaltyScoreRole)
        return chronoItem.getPenaltyScore();
    else if (role == FinalScoreRole)
        return chronoItem.getFinalScore();
    else if (role == GymnastTotalScoreRole)
        return chronoItem.getTotScore();
    else if (role == LatestScoreRole)
        return chronoItem.isLatestScore();
//    else if (role == RankRole)
//        return chronoItem.getRank();
    else
        qDebug() << "Role " << role << " not found";

    return QVariant();
}

QHash<int, QByteArray> ChronoListDataModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[MenWomenRole         ]      = "MenWomen";
    roles[NameFullRole         ]      = "NameFull";
    roles[FlagImageRole        ]      = "FlagImage";
    roles[CountryRole          ]      = "Country";
    roles[ApparatusRole        ]      = "Apparatus";
    roles[DifficultyScoreRole  ]      = "DifficultyScore";
    roles[ExecutionScoreRole   ]      = "ExecutionScore";
    roles[BonusScoreRole       ]      = "BonusScore";
    roles[PenaltyScoreRole     ]      = "PenaltyScore";
    roles[FinalScoreRole       ]      = "FinalScore";
    roles[GymnastTotalScoreRole]      = "GymnastTotalScore";
    roles[LatestScoreRole      ]      = "LatestScore";
//    roles[RankRole             ]      = "Rank";

    return roles;
}

QModelIndex ChronoListDataModel::indexFromItem(const ChronoListData* item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_chronoList.size(); ++row)
    {
        if(m_chronoList.at(row) == (*item)) return index(row);
    }
    return QModelIndex();
}

ChronoListData* ChronoListDataModel::GetItem(QString& nameFull)
{
    QList<ChronoListData>::iterator iter;
    for (iter = m_chronoList.begin(); iter != m_chronoList.end(); ++iter)
    {
        if ((iter)->getNameFull() == nameFull)
            return &(*iter);
    }

    return nullptr;
}

bool ChronoListDataModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    Q_UNUSED(source_row);
    Q_UNUSED(source_parent);

    return true;
}
