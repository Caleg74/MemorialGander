#include "gymnastdatamodel.h"
#include "dbinterface.h"
#include "gymnastselectionlist.h"
#include <QAbstractItemModel>

//**** STATIC MEMBER INITIALIZATION *********************
GymnastDataModel* GymnastDataModel::sm_pInstance = NULL;


//*******************************************************

GymnastDataModel* GymnastDataModel::Instance()
{
    if (sm_pInstance == NULL)
    {
        sm_pInstance = new GymnastDataModel();
    }

    return sm_pInstance;
}

GymnastDataModel::GymnastDataModel(QObject *parent)
    : QAbstractListModel(parent)
{
    RetrieveGymnastList();
}

void GymnastDataModel::RetrieveGymnastList()
{
    QList<QStringList> p_strGymnList;
    dbInterface::Instance()->retrieveRegisteredGymnastList(p_strGymnList);

    for (int i = 0; i < p_strGymnList.size();i++)
    {
        // Convert NationId to nicename
        QString strNationName = dbInterface::Instance()->getNationName(p_strGymnList.at(i)[2].toInt(), dbIfaceBase::NI_Nicename);

        GymnastData cNewGymnast(p_strGymnList.at(i)[0], p_strGymnList.at(i)[1],
                strNationName, p_strGymnList.at(i)[3]);


        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_gymnastList << cNewGymnast;
        endInsertRows();
    }
}

bool GymnastDataModel::addItem(QString firstName,
                              QString lastName,
                              QString country,
                              QString sex)
{

    int iNationId = dbInterface::Instance()->getNationId(country);

    GymnastData cNewGymnast(firstName, lastName, country, sex);

    // Keep the UI model in sync with the database if the insert fails.
    if (!dbInterface::Instance()->insertGymnast(firstName, lastName, sex, iNationId))
        return false;

    // update combo box for gymnast selection
    GymnastSelectionList::Instance()->FillList();

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_gymnastList << cNewGymnast;
    endInsertRows();

    return true;
}

void GymnastDataModel::removeItem(QString firstName, QString lastName)
{
    GymnastData* pItem = GetItem(firstName, lastName);

    QModelIndex pIndex = indexFromItem(pItem);

    // add it to the database
    dbInterface::Instance()->deleteGymnast(firstName, lastName);

    // update combo box for gymnast selection
    GymnastSelectionList::Instance()->FillList();

    beginRemoveRows(QModelIndex(), pIndex.row(), pIndex.row());
    m_gymnastList.removeAt(pIndex.row());
    endRemoveRows();
}

int GymnastDataModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_gymnastList.count();
}

QVariant GymnastDataModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_gymnastList.count())
        return QVariant();

    const GymnastData &gymnast = m_gymnastList[index.row()];
    if (role == FirstNameRole)
        return gymnast.FirstName();
    else if (role == LastNameRole)
        return gymnast.LastName();
    else if (role == CountryRole)
        return gymnast.Country();
    else if (role == SexRole)
        return gymnast.Gender();
    return QVariant();
}

QHash<int, QByteArray> GymnastDataModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FirstNameRole] = "FirstName";
    roles[LastNameRole]  = "LastName";
    roles[CountryRole]   = "Country";
    roles[SexRole]       = "Sex";

    return roles;
}

QModelIndex GymnastDataModel::indexFromItem(const GymnastData* item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_gymnastList.size(); ++row)
    {
        if(m_gymnastList.at(row) == (*item)) return index(row);
    }
    return QModelIndex();
}

GymnastData* GymnastDataModel::GetItem(QString& firstName, QString& lastName)
{
    QList<GymnastData>::iterator iter;
    for (iter = m_gymnastList.begin(); iter != m_gymnastList.end(); ++iter)
    {
        if (((iter)->FirstName() == firstName) && ((iter)->LastName() == lastName))
            return &(*iter);
    }

    return NULL;
}
