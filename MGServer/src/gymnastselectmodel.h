#ifndef GYMNASTSELECTMODEL_H
#define GYMNASTSELECTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include "gymnastselectdata.h"

class GymnastSelectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum GymnastRoles {
        FullNameRole = Qt::UserRole + 1,
        AthleteIdRole,
        EventIdRole,
        ApparatusListRole,
        FinalApparatusIndexRole
    };

    /** Singleton */
    static GymnastSelectModel* Instance();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public slots:

    bool addItem(QString gymnastFullTxt);

    void removeItem(QString gymnastFullTxt);

    bool setFinalApparatus(int athleteId, int eventId, int apparatusIndex);

protected:
    QHash<int, QByteArray> roleNames() const;

private:

    GymnastSelectData* GetItem(QString& gymnastFullTxt);

    QModelIndex indexFromItem( const GymnastSelectData* item) const;

    GymnastSelectModel(QObject *parent = 0);

    void RetrieveGymnastList();

    static GymnastSelectModel* sm_pInstance;   ///< Singleton

    QList<GymnastSelectData> m_gymnastList;
};

#endif // GYMNASTSELECTMODEL_H
