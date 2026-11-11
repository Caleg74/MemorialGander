#include "coreapplication.h"
#include <QQmlContext>
#include "gymnastdatamodel.h"
#include "gymnastselectmodel.h"
#include "dbinterface.h"
#include <QQuickItem>
#include "apparatuslist.h"
#include "messagebox.h"
#include "saveforcescore.h"
#include <algorithm>
#include <iterator>

CoreApplication::CoreApplication(QObject *parent)
    : QObject(parent)
{
}

QList<int> CoreApplication::editions() const
{
    return {2016, 2018, 2022, 2024, 2026, 2028};
}

int CoreApplication::getCurrentEvent() const
{
    return dbInterface::Instance()->getCurrentEvent();
}

bool CoreApplication::selectEvent(int event)
{
    if (!editions().contains(event))
        return false;

    auto *database = dbInterface::Instance();
    if (!database->saveActiveEvent(event)) {
        MessageBox message;
        message.SetTitle(tr("Edizione"));
        message.SetText(tr("Salvataggio dell'edizione %1 non riuscito.").arg(event));
        message.Show();
        return false;
    }
    if (event == getCurrentEvent())
        return true;

    database->setCurrentEvent(event);
    if (m_qSortProxySelectedGymnast)
        GymnastSelectModel::Instance()->reload();

    emit currentEventChanged();
    return true;
}

void CoreApplication::Init(QQmlApplicationEngine& p_qEngine)
{
    auto *database = dbInterface::Instance();
    bool bDBOk = true;
    // Init the DB connection
    if (database->loadDriver())
    {
        qInfo() << "QOBCD driver loaded successfully";
    }
    else
    {
        bDBOk = false;
    }

    // initialize the database
    if (!database->initDb(false))
    {
        bDBOk = false;
    }

    const auto availableEditions = editions();
    int initialEdition = database->getActiveEventYear();
    if (!availableEditions.contains(initialEdition)) {
        const auto nextEdition = std::upper_bound(availableEditions.cbegin(),
                                                  availableEditions.cend(), getCurrentEvent());
        initialEdition = nextEdition == availableEditions.cbegin()
                             ? availableEditions.constFirst() : *std::prev(nextEdition);
    }
    database->setCurrentEvent(initialEdition);

    // save it as a class member
    m_pAppEngine = &p_qEngine;
    MessageBox::SetEngine(m_pAppEngine);

    QQmlContext *ctxt = p_qEngine.rootContext();

    if (bDBOk)
    {
        // Create the the model for the gymnasts of every event
        GymnastDataModel* pGymModel = GymnastDataModel::Instance();
        // Set filter
        m_qSortProxyGymnast = new GymnastSortFilterProxyModel(this);
        m_qSortProxyGymnast->setSortRole(GymnastDataModel::FirstNameRole);
        m_qSortProxyGymnast->setSourceModel(pGymModel);
        m_qSortProxyGymnast->setDynamicSortFilter(true);
        ctxt->setContextProperty("GymnastDataModel", m_qSortProxyGymnast); // Contains the filter, not the model
        m_qSortProxyGymnast->sort(0);

        // Create the "registers to this event gymnasts" model
        GymnastSelectModel* pGymSelModel = GymnastSelectModel::Instance();
        // Set filter
        m_qSortProxySelectedGymnast = new GymnastSortFilterProxyModel(this);
        m_qSortProxySelectedGymnast->setSortRole(GymnastDataModel::FirstNameRole);
        m_qSortProxySelectedGymnast->setSourceModel(pGymSelModel);
        m_qSortProxySelectedGymnast->setDynamicSortFilter(true);
        ctxt->setContextProperty("GymnastSelectModel", m_qSortProxySelectedGymnast);
        m_qSortProxySelectedGymnast->sort(0);

        ApparatusList::Instance()->Init(p_qEngine, "apparatusModel", &m_pApparatusCbbModel);

        m_cPdf.retriveGymnastList();
    }
}

void CoreApplication::Connect()
{
    QObject* saveScoreBtn = m_pAppEngine->rootObjects().first()->findChild<QObject*>("btnSaveScore");
    if (saveScoreBtn)
    {
        connect(saveScoreBtn, SIGNAL(saveScore(QString, QString, QString)),
                SaveForceScore::Instance(), SLOT(onSaveForceScore(QString, QString, QString)));
    }

    QObject* eraseForcedScoreBtn = m_pAppEngine->rootObjects().first()->findChild<QObject*>("btnEraseForceScore");
    if (eraseForcedScoreBtn)
    {
        connect(eraseForcedScoreBtn, SIGNAL(clicked()),
                SaveForceScore::Instance(), SLOT(eraseForcedScores()));
    }

    QQuickItem* cbbGymnast = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("cbbGymnastSelection");
    if (cbbGymnast)
    {
        connect(cbbGymnast, SIGNAL(selectedTextChanged(QString)),
                this, SLOT(onGymnastChanged(QString)));
    }

    QObject* createRankingPdfBtn = m_pAppEngine->rootObjects().first()->findChild<QObject*>("btnCreatePdf");
    if (createRankingPdfBtn)
    {
        connect(createRankingPdfBtn, SIGNAL(clicked()),
                &m_cPdf, SLOT(PrintRanking()));
    }

    QObject* createLastApparatusPdfBtn = m_pAppEngine->rootObjects().first()->findChild<QObject*>("btnCreateLastApparatusPdf");
    if (createLastApparatusPdfBtn)
    {
        connect(createLastApparatusPdfBtn, SIGNAL(clicked()),
                &m_cPdf, SLOT(PrintLastApparatusStartOrder()));
    }

}

void CoreApplication::onGymnastChanged(QString p_currentTxt)
{
    if (p_currentTxt != "Seleziona ginnasti..")
    {
        QStringList strSplit = p_currentTxt.split(',');
        if (strSplit.count() < 2)
        {
            qDebug() << "onGymnastChanged(): first/last name formatting error";
        }
        QString firstName = strSplit.at(0).trimmed();
        QString lastName =  strSplit.at(1).trimmed();

        if (!firstName.isEmpty() && !lastName.isEmpty())
        {
            int iAthleteId = dbInterface::Instance()->getGymnastId(firstName, lastName);
            QString strGender = dbInterface::Instance()->getGender(iAthleteId);

            ApparatusList::Instance()->FillComboList(&m_pApparatusCbbModel, strGender);
        }
        else
        {
            qDebug() << "onGymnastChanged(): first/last name is empty";
        }
    }
}
