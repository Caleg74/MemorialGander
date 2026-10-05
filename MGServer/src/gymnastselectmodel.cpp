#include "GymnastSelectModel.h"
#include <QDebug>
#include "dbinterface.h"

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
    QList<QStringList> p_strGymnList;
    dbInterface::Instance()->retrieveGymnastSubscriptionList(p_strGymnList);

    for (int i = 0; i < p_strGymnList.size();i++)
    {
        QString strGymnFullTxt = p_strGymnList.at(i)[2] + ", "
                               + p_strGymnList.at(i)[3] + ", "
                               + "(" + p_strGymnList.at(i)[4] + ")";

        GymnastSelectData cNewGymnast(p_strGymnList.at(i)[0].toInt(),
                                      p_strGymnList.at(i)[1].toInt(),
                                      strGymnFullTxt);

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
        GymnastSelectData cNewGymnast(iAthleteId, iEventId, gymnastFullTxt);

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
    return QVariant();
}

QHash<int, QByteArray> GymnastSelectModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FullNameRole]   = "FullName";
    roles[AthleteIdRole]  = "AthleteId";
    roles[EventIdRole]    = "EventId";

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
