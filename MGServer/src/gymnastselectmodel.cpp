#include "GymnastSelectModel.h"
#include <QDebug>
#include "apparatuslist.h"
#include "dbinterface.h"
#include <utility>

//**** STATIC MEMBER INITIALIZATION *********************
GymnastSelectModel* GymnastSelectModel::sm_pInstance = NULL;


//*******************************************************

GymnastSelectModel* GymnastSelectModel::Instance()
{
    if (sm_pInstance == NULL)
    {
        sm_pInstance = new GymnastSelectModel();
    }

    return sm_pInstance;
}

GymnastSelectModel::GymnastSelectModel(QObject *parent)
    : QAbstractListModel(parent)
{
    RetrieveGymnastList();
}

void GymnastSelectModel::RetrieveGymnastList()
{
    ApparatusList::Instance()->load();

    QList<QStringList> p_strGymnList;
    dbInterface::Instance()->retrieveGymnastSubscriptionList(p_strGymnList);

    for (const QStringList &gymnastData : std::as_const(p_strGymnList)) {
        if (gymnastData.size() < 7) {
            qCritical() << "RetrieveGymnastList(): incomplete gymnast data" << gymnastData;
            continue;
        }

        const QString gymnastFullText = gymnastData[2] + ", "
                                      + gymnastData[3] + ", "
                                      + "(" + gymnastData[4] + ")";
        const QString gender = gymnastData[5];
        const QStringList apparatusList = ApparatusList::Instance()->comboListForGender(gender);
        const QString finalApparatusName = ApparatusList::Instance()->apparatusName(gymnastData[6].toInt());
        const int finalApparatusIndex = qMax(0, apparatusList.indexOf(finalApparatusName));

        GymnastSelectData cNewGymnast(gymnastData[0].toInt(),
                                      gymnastData[1].toInt(),
                                      gymnastFullText,
                                      gender,
                                      apparatusList,
                                      finalApparatusIndex);

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_gymnastList << cNewGymnast;
        endInsertRows();
    }
}

bool GymnastSelectModel::addItem(QString gymnastFullTxt)
{
    QStringList strSplit = gymnastFullTxt.split(',');
    if (strSplit.count() < 2)
    {
        qCritical() << "addItem(): first/last name formatting error";
        return false;
    }
    QString firstName = strSplit.at(0).trimmed();
    QString lastName =  strSplit.at(1).trimmed();

    int iAthleteId = dbInterface::Instance()->getGymnastId(firstName, lastName);
    int iEventId = dbInterface::Instance()->getCurrentEventId();

    if (!dbInterface::Instance()->isGymnastAlreadyRegistered(iAthleteId, iEventId))
    {
        const QString gender = dbInterface::Instance()->getGender(iAthleteId);
        const QStringList apparatusList = ApparatusList::Instance()->comboListForGender(gender);
        GymnastSelectData cNewGymnast(iAthleteId, iEventId, gymnastFullTxt,
                                      gender, apparatusList);

        // Keep the UI model in sync with the database if the subscription fails.
        if (!dbInterface::Instance()->subscribeGymnasttoEvent(iAthleteId, iEventId))
            return false;

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_gymnastList << cNewGymnast;
        endInsertRows();

        return true;
    }
    else
    {
        qWarning() << "Athlete " << gymnastFullTxt << " already present";
    }

    return false;
}

void GymnastSelectModel::removeItem(QString gymnastFullTxt)
{
    GymnastSelectData* pItem = GetItem(gymnastFullTxt);

    QModelIndex pIndex = indexFromItem(pItem);

    // remove it from the db
    QStringList strSplit = gymnastFullTxt.split(',');
    if (strSplit.count() < 2)
    {
        qCritical() << "removeItem(): first/last name formatting error";
        return;
    }
    QString firstName = strSplit.at(0).trimmed();
    QString lastName =  strSplit.at(1).trimmed();

    int iAthleteId = dbInterface::Instance()->getGymnastId(firstName, lastName);
    int iEventId = dbInterface::Instance()->getCurrentEventId();

    // remove it from the database
    dbInterface::Instance()->deleteGymnastSubsscription(iAthleteId, iEventId);

    beginRemoveRows(QModelIndex(), pIndex.row(), pIndex.row());
    m_gymnastList.removeAt(pIndex.row());
    endRemoveRows();
}

bool GymnastSelectModel::setFinalApparatus(int athleteId, int eventId, int apparatusIndex)
{
    for (int row = 0; row < m_gymnastList.size(); ++row) {
        auto &gymnast = m_gymnastList[row];
        if (gymnast.GymnastId() != athleteId || gymnast.EventId() != eventId)
            continue;

        const QStringList apparatusList = gymnast.ApparatusList();
        if (apparatusIndex < 0 || apparatusIndex >= apparatusList.size()) {
            qWarning() << "Invalid apparatus index" << apparatusIndex << "for athlete" << athleteId;
            return false;
        }

        int apparatusId = 0;
        if (apparatusIndex > 0) {
            apparatusId = ApparatusList::Instance()->getApparatusId(
                apparatusList[apparatusIndex], gymnast.Gender());
            if (apparatusId <= 0)
                return false;
        }

        if (!dbInterface::Instance()->updateFinalApparatus(athleteId, eventId, apparatusId))
            return false;

        if (gymnast.FinalApparatusIndex() != apparatusIndex) {
            gymnast.SetFinalApparatusIndex(apparatusIndex);
            const QModelIndex changedIndex = index(row);
            emit dataChanged(changedIndex, changedIndex, {FinalApparatusIndexRole});
        }
        return true;
    }

    qWarning() << "Athlete subscription not found:" << athleteId << eventId;
    return false;
}

int GymnastSelectModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_gymnastList.count();
}

QVariant GymnastSelectModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_gymnastList.count())
        return QVariant();

    const GymnastSelectData &gymnast = m_gymnastList[index.row()];
    if (role == FullNameRole)
        return gymnast.GymnastFullTxt();
    else if (role == AthleteIdRole)
        return gymnast.GymnastId();
    else if (role == EventIdRole)
        return gymnast.EventId();
    else if (role == ApparatusListRole)
        return gymnast.ApparatusList();
    else if (role == FinalApparatusIndexRole)
        return gymnast.FinalApparatusIndex();
    return QVariant();
}

QHash<int, QByteArray> GymnastSelectModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FullNameRole] = "fullName";
    roles[AthleteIdRole] = "athleteId";
    roles[EventIdRole] = "eventId";
    roles[ApparatusListRole] = "apparatusList";
    roles[FinalApparatusIndexRole] = "finalApparatusIndex";

    return roles;
}

QModelIndex GymnastSelectModel::indexFromItem(const GymnastSelectData *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_gymnastList.size(); ++row)
    {
        if(m_gymnastList.at(row) == (*item)) return index(row);
    }
    return QModelIndex();
}

GymnastSelectData* GymnastSelectModel::GetItem(QString& gymnastFullTxt)
{
    QList<GymnastSelectData>::iterator iter;
    for (iter = m_gymnastList.begin(); iter != m_gymnastList.end(); ++iter)
    {
        if ((iter)->GymnastFullTxt() == gymnastFullTxt)
            return &(*iter);
    }

    return NULL;
}
