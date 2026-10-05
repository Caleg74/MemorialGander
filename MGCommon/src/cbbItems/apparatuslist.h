#ifndef APPARATUSLIST_H
#define APPARATUSLIST_H

#include "comboboxmodel.h"
#include <QQmlApplicationEngine>
#include "apparatusdata.h"


class ApparatusList : public QObject
{
    Q_OBJECT
public:

    enum EApparatusGeneral
    {
        AGTotalScore,
        AGFirstApparatus
    };

    enum EApparatusMen
    {
        AMFloor = AGFirstApparatus,
        AMPommelHorse,
        AMRings,
        AMVault,
        AMParallelBars,
        AMHorizontalBar,
        AMApparatusMax
    };

    enum EApparatusWomen
    {
        AWVault = AGFirstApparatus,
        AWUnevenBars,
        AWBalanceBeam,
        AWFloor,
        AWApparatusMax
    };

    static ApparatusList* Instance();

    ~ApparatusList();

    /** Initializes the core application components */
    void Init(QQmlApplicationEngine &p_qEngine, const QString& p_strPropertyName, ComboBoxModel * const p_pCbbModel);

    void load();

    void FillComboList(ComboBoxModel * const p_pCbbModel, QString p_strGender, bool p_bNoSelectionPresent = true);

    QStringList comboListForGender(const QString &gender, bool includePlaceholder = true) const;

    QString apparatusName(int apparatusId) const;

    int getApparatusId(EApparatusMen p_eApparatusMen);
    int getApparatusId(EApparatusWomen p_eApparatusWomen);
    int getApparatusId(const QString& p_strApparatusName, const QString &p_strGender);


private:

    ApparatusList();

    void RetrieveFromDb();

    static ApparatusList* sm_pInstance;

    QList<ApparatusData*> m_ApparatusList;

    int m_aiEnumToIdMen[AMApparatusMax] = {};

    int m_aiEnumToIdWomen[AWApparatusMax] = {};

};

#endif // APPARATUSLIST_H
