#ifndef ALLROUNDWOMENDATAMODEL_H
#define ALLROUNDWOMENDATAMODEL_H

#include "gymnastdatamodel.h"
#include "athletedata.h"

class AllroundWomenDataModel : public GymnastDataModel
{
    Q_OBJECT
public:
    enum AllroundWRoles {
        DifficultyScoreTotalRole = LastGeneralRole,
        FinalScoreTotalRole,
        DifficultyScoreVaultRole,
        FinalScoreVaultRole,
        FinalApparatusVaultRole,
        LatestScoreVaultRole,
        DifficultyScoreUnBarsRole,
        FinalScoreUnBarsRole,
        FinalApparatusUnBarsRole,
        LatestScoreUnBarsRole,
        DifficultyScoreBalBeamRole,
        FinalScoreBalBeamRole,
        FinalApparatusBalBeamRole,
        LatestScoreBalBeamRole,
        DifficultyScoreFloorRole,
        FinalScoreFloorRole,
        FinalApparatusFloorRole,
        LatestScoreFloorRole
     };

    AllroundWomenDataModel(QObject *parent = 0);

    ~AllroundWomenDataModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

//    bool insertRow(int row, const QModelIndex & parent = QModelIndex());


public slots:

    void updateScores();

//    void addItem(QString nameFull);

//    void removeItem(QString nameFull);

protected:
    QHash<int, QByteArray> roleNames() const;

private:

    AthleteData *GetItem(QString& nameFull);

    QModelIndex indexFromItem(const AthleteData *item) const;

    void RetrieveGymnastList();

    QList<AthleteData> m_rankingList;
};

#endif // ALLROUNDWOMENDATAMODEL_H
