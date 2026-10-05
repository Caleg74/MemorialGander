#ifndef GYMNASTDATAMODEL_H
#define GYMNASTDATAMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "gymnastdata.h"

class GymnastDataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum GymnastRoles {
        FirstNameRole = Qt::UserRole + 1,
        LastNameRole,
        CountryRole,
        SexRole
    };

    /** Singleton */
    static GymnastDataModel* Instance();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//    bool insertRow(int row, const QModelIndex & parent = QModelIndex());


public slots:

    bool addItem(QString firstName,
                 QString lastName,
                 QString country,
                 QString sex);

    void removeItem(QString firstName, QString lastName);

protected:
    QHash<int, QByteArray> roleNames() const;

private:

    GymnastData* GetItem(QString& firstName, QString& lastName);

    QModelIndex indexFromItem( const GymnastData* item) const;

    GymnastDataModel(QObject *parent = 0);

    void RetrieveGymnastList();

    static GymnastDataModel* sm_pInstance;   ///< Singleton

    QList<GymnastData> m_gymnastList;
};

#endif // GYMNASTDATAMODEL_H
