#include "apparatuslist.h"
#include <QQmlContext>
#include "dbinterface.h"
#include <QDebug>

// Static membe rinitialization
ApparatusList* ApparatusList::sm_pInstance = NULL;

ApparatusList::ApparatusList()
{
}

ApparatusList* ApparatusList::Instance()
{
    if (sm_pInstance == NULL)
    {
       sm_pInstance = new ApparatusList();
    }

    return sm_pInstance;
}

ApparatusList::~ApparatusList()
{
    // remove all items
    for (int i=0; i<m_ApparatusList.count(); ++i)
    {
        delete m_ApparatusList.at(i);
    }
    m_ApparatusList.clear();
}

void ApparatusList::Init(QQmlApplicationEngine &p_qEngine, const QString& p_strPropertyName, ComboBoxModel* const p_pCbbModel)
{
    QQmlContext* pComboContext = p_qEngine.rootContext();
    pComboContext->setContextProperty(p_strPropertyName, p_pCbbModel);

    load();
}

void ApparatusList::load()
{
    if (m_ApparatusList.isEmpty())
    {
        RetrieveFromDb();
    }
}

void ApparatusList::RetrieveFromDb()
{
    QStringList tmp;
    dbInterface::Instance()->getApparatusList(&tmp);

    for (int i = 0; i < tmp.size(); ++i)
    {
        QStringList items = tmp.at(i).split(',');

        if (items.count() == 4) // 4 items retrieved
        {
            m_ApparatusList << new ApparatusData(items.at(0).toInt(),   // id
                                                 items.at(1),           // name_it
                                                 items.at(2),           // name_en
                                                 items.at(3));           // gender

            if ((items.at(2) == "Floor") && (items.at(3) == "M")) m_aiEnumToIdMen[AMFloor] = items.at(0).toInt();
            else if ((items.at(2) == "Pommel horse") && (items.at(3) == "M")) m_aiEnumToIdMen[AMPommelHorse] = items.at(0).toInt();
            else if ((items.at(2) == "Rings") && (items.at(3) == "M")) m_aiEnumToIdMen[AMRings] = items.at(0).toInt();
            else if ((items.at(2) == "Vault") && (items.at(3) == "M")) m_aiEnumToIdMen[AMVault] = items.at(0).toInt();
            else if ((items.at(2) == "Parallel bars") && (items.at(3) == "M")) m_aiEnumToIdMen[AMParallelBars] = items.at(0).toInt();
            else if ((items.at(2) == "Horizontal bar") && (items.at(3) == "M")) m_aiEnumToIdMen[AMHorizontalBar] = items.at(0).toInt();
            else if ((items.at(2) == "Vault") && (items.at(3) == "F")) m_aiEnumToIdWomen[AWVault] = items.at(0).toInt();
            else if ((items.at(2) == "Uneven bars") && (items.at(3) == "F")) m_aiEnumToIdWomen[AWUnevenBars] = items.at(0).toInt();
            else if ((items.at(2) == "Balance beam") && (items.at(3) == "F")) m_aiEnumToIdWomen[AWBalanceBeam] = items.at(0).toInt();
            else if ((items.at(2) == "Floor") && (items.at(3) == "F")) m_aiEnumToIdWomen[AWFloor] = items.at(0).toInt();

        }
        else
        {
            qCritical() << "RetrieveFromDb(): items count incorrect";
        }
    }
}

void ApparatusList::FillComboList(ComboBoxModel* const p_pCbbModel, QString p_strGender, bool p_bNoSelectionPresent)
{
    p_pCbbModel->setComboList(comboListForGender(p_strGender, p_bNoSelectionPresent));
}

QStringList ApparatusList::comboListForGender(const QString &gender, bool includePlaceholder) const
{
    QStringList apparatusNames;
    if (includePlaceholder)
        apparatusNames << QStringLiteral("Attrezzo..");

    for (const auto *apparatus : m_ApparatusList) {
        if (apparatus->getGender() == gender)
            apparatusNames << apparatus->getName(ApparatusData::ALangIt);
    }

    return apparatusNames;
}

QString ApparatusList::apparatusName(int apparatusId, ApparatusData::ApparatusLanguage_t p_eLang) const
{
    for (const auto *apparatus : m_ApparatusList) {
        if (apparatus->getId() == apparatusId)
            return apparatus->getName(p_eLang);
    }

    return {};
}

int ApparatusList::getApparatusId(EApparatusMen p_eApparatusMen)
{
    return m_aiEnumToIdMen[p_eApparatusMen];
}

int ApparatusList::getApparatusId(EApparatusWomen p_eApparatusWomen)
{
    return m_aiEnumToIdWomen[p_eApparatusWomen];
}

int ApparatusList::getApparatusId(const QString& p_strApparatusName, const QString& p_strGender)
{
    int iApparatusId = 0;
    bool bFound= false;

    for (int i=0; i<m_ApparatusList.count(); i++)
    {
        if (m_ApparatusList.at(i)->getGender() == p_strGender)
        {
            if ( (m_ApparatusList.at(i)->getName(ApparatusData::ALangIt) == p_strApparatusName)
              || (m_ApparatusList.at(i)->getName(ApparatusData::ALangEn) == p_strApparatusName))
            {
                iApparatusId = m_ApparatusList.at(i)->getId();
                bFound = true;
                break;
            }
        }
    }

    if (!bFound)
    {
        qCritical() << "No ApparatusId found for "  << p_strApparatusName;
    }

    return iApparatusId;
}
