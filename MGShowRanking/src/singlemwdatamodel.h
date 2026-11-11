#ifndef SINGLEMWDATAMODEL_H
#define SINGLEMWDATAMODEL_H

#include "gymnastdatamodel.h"
#include "singlemwdata.h"
#include "apparatuslist.h"

class SingleMWDataModel : public GymnastDataModel
{
    Q_OBJECT
public:
    enum SingleMWRoles
    {
        FinalScoreTotalRole = LastGeneralRole,
        ScoreDetailsSingleMW,
        FinalScoreSingleMW
     };

    enum SingleGender
    {
        SG_Unknown,
        SG_Men,
        SG_Women
    };

    explicit SingleMWDataModel(const QString& p_strGender, QObject *parent = nullptr);

    ~SingleMWDataModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

//    bool insertRow(int row, const QModelIndex & parent = QModelIndex());

    void setApparatus(ApparatusList::EApparatusMen p_eAppMen);

    void setApparatus(ApparatusList::EApparatusWomen p_eAppWomen);

public slots:

    void updateScores();

//    void addItem(QString nameFull);

//    void removeItem(QString nameFull);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:

    SingleMWData* GetItem(QString& nameFull);

    QModelIndex indexFromItem( const SingleMWData* item) const;

    void RetrieveGymnastList();

    SingleGender m_eGender;

    int m_iApparatus = ApparatusList::AGFirstApparatus;

    QList<SingleMWData> m_rankingList;
};

#endif // SINGLEMWDATAMODEL_H
