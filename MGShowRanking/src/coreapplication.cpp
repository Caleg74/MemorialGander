#include "coreapplication.h"
#include <QQmlContext>
#include "sortfilterproxymodel.h"
#include "dbinterface.h"
#include <QQuickItem>
#include <QDebug>

CoreApplication::CoreApplication(QObject *parent)
    : QObject(parent)
    , m_pGymWomenModel(nullptr)
    , m_pGymMenModel(nullptr)
    , m_pSingleMModel(nullptr)
    , m_pSingleWModel(nullptr)
    , m_pChronoListModel(nullptr)
    , m_pAppEngine(nullptr)
    , m_qSortMProxy(nullptr)
    , m_qSortWProxy(nullptr)
    , m_qSortSingleMProxy(nullptr)
    , m_qSortSingleWProxy(nullptr)
    , m_qSortChronoProxy(nullptr)
    , m_updateTimer(nullptr)
{

}

CoreApplication::~CoreApplication()
{
    delete m_pGymWomenModel;
    delete m_pGymMenModel;
    delete m_pSingleMModel;
    delete m_pSingleWModel;
    delete m_pChronoListModel;
    delete m_qSortMProxy;
    delete m_qSortWProxy;
    delete m_updateTimer;
    delete m_qSortSingleMProxy;
    delete m_qSortSingleWProxy;
    delete m_qSortChronoProxy;
    m_pGymWomenModel = nullptr;
    m_pGymMenModel = nullptr;
    m_pSingleMModel = nullptr;
    m_pSingleWModel = nullptr;
    m_pChronoListModel = nullptr;
    m_qSortMProxy = nullptr;
    m_qSortWProxy = nullptr;
    m_updateTimer = nullptr;
    m_qSortSingleMProxy = nullptr;
    m_qSortSingleWProxy = nullptr;
    m_qSortChronoProxy = nullptr;
}

bool CoreApplication::Init(QQmlApplicationEngine& p_qEngine)
{
    auto *database = dbInterface::Instance();
    if (database->loadDriver())
    {
        qInfo() << "QOBCD driver loaded successfully";
    }
    else
    {
        return false;
    }

    // initialize the database
    if (!database->initDb(true))
    {
        return false;
    }

    if (const int year = database->getActiveEventYear(); year > 0)
        database->setCurrentEvent(year);

    // save it as a class member
    m_pAppEngine = &p_qEngine;
    QQmlContext *ctxt = p_qEngine.rootContext();


    // Men
    ApparatusList::Instance()->Init(p_qEngine, "apparatusModelM", &m_cbbApparatusMen);
    ApparatusList::Instance()->FillComboList(&m_cbbApparatusMen, "M", false);

    m_pGymMenModel = new AllroundMenDataModel();

    // Set filter
    m_qSortMProxy = new SortFilterProxyModel(this);
    m_qSortMProxy->setSortRole(AllroundMenDataModel::RankRole);
    m_qSortMProxy->setSourceModel(m_pGymMenModel);
    m_qSortMProxy->setDynamicSortFilter(true);

    ctxt->setContextProperty("AllroundMenDataModel", m_qSortMProxy); // Contains the filter, not the model
    m_qSortMProxy->sort(0);

    // Women
    m_pGymWomenModel =  new AllroundWomenDataModel();

    // Set filter
    ApparatusList::Instance()->Init(p_qEngine, "apparatusModelW", &m_cbbApparatusWomen);
    ApparatusList::Instance()->FillComboList(&m_cbbApparatusWomen, "F", false);

    m_qSortWProxy = new SortFilterProxyModel(this);
    m_qSortWProxy->setSortRole(AllroundWomenDataModel::RankRole);
    m_qSortWProxy->setSourceModel(m_pGymWomenModel);
    m_qSortWProxy->setDynamicSortFilter(true);

    ctxt->setContextProperty("AllroundWomenDataModel", m_qSortWProxy); // Contains the filter, not the model
    m_qSortWProxy->sort(0);

    // Single Men model
    m_pSingleMModel = new SingleMWDataModel("M");

    // Set filter
    m_qSortSingleMProxy = new SortFilterProxyModel(this);
    m_qSortSingleMProxy->setSortRole(SingleMWDataModel::RankRole);
    m_qSortSingleMProxy->setFilterRole(SingleMWDataModel::FinalScoreSingleMW);
    m_qSortSingleMProxy->setSourceModel(m_pSingleMModel);
    m_qSortSingleMProxy->setDynamicSortFilter(true);

    ctxt->setContextProperty("SingleMDataModel", m_qSortSingleMProxy); // Contains the filter, not the model
    m_qSortSingleMProxy->sort(0);

    // Single Women model
    m_pSingleWModel = new SingleMWDataModel("F");

    // Set filter
    m_qSortSingleWProxy = new SortFilterProxyModel(this);
    m_qSortSingleWProxy->setSortRole(SingleMWDataModel::RankRole);
    m_qSortSingleWProxy->setFilterRole(SingleMWDataModel::FinalScoreSingleMW);
    m_qSortSingleWProxy->setSourceModel(m_pSingleWModel);
    m_qSortSingleWProxy->setDynamicSortFilter(true);

    ctxt->setContextProperty("SingleWDataModel", m_qSortSingleWProxy); // Contains the filter, not the model
    m_qSortSingleWProxy->sort(0);
//    // Timer for refreshing the scores automatically


//    connect(pGymModel, SIGNAL(OutputChanged(unsigned int)),
//            &m_cIoWrap, SLOT(SetOutput(unsigned int)));

    // Chronological list, sorted already within the DB view
    m_pChronoListModel = new ChronoListDataModel();
    m_qSortChronoProxy = new SortFilterProxyModel(this);
    m_qSortChronoProxy->setSourceModel(m_pChronoListModel);
    m_qSortChronoProxy->setDynamicSortFilter(true);
    ctxt->setContextProperty("ChronoListModel", m_pChronoListModel); // Contains the filter, not the model

    return true;
}

void CoreApplication::Connect()
{
    QObject *topLevel = m_pAppEngine->rootObjects().value(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if (window)
    {
        connect(topLevel, SIGNAL(afterRenderPassRecording()), this, SLOT(onWindowLoaded()));
        connect(topLevel, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onWindowClosing(QQuickCloseEvent*)));
    }

    QQuickItem* cbbApparatusMen = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("cbbAppartusM");
    if (cbbApparatusMen)
    {
        connect(cbbApparatusMen, SIGNAL(selectedTextChanged(QString)),
                this, SLOT(onApparatusMChanged(QString)));

    }

    QQuickItem* cbbApparatusWomen = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("cbbAppartusW");
    if (cbbApparatusMen)
    {
        connect(cbbApparatusWomen, SIGNAL(selectedTextChanged(QString)),
                this, SLOT(onApparatusWChanged(QString)));

    }
}
void CoreApplication::onWindowLoaded()
{
    if (m_updateTimer == nullptr)
    {
        m_updateTimer = new QTimer();
        connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updateScores()));
        m_updateTimer->start(2000); //time specified in ms
    }
}

void CoreApplication::onWindowClosing(QQuickCloseEvent* p_event)
{
    Q_UNUSED(p_event);

    qDebug() << "onWindowClosing()";
    if (m_updateTimer != nullptr)
    {
        m_updateTimer->stop();
        m_updateTimer->disconnect();    // disconnect all signals
    }
}

void CoreApplication::updateScores()
{
        QQuickItem* qMenRankingTab = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("idMenRanking");
        if (qMenRankingTab == nullptr)
        {
            qCritical() << "Item idMenRanking not found";
            return;
        }

        QQuickItem* qWomenRankingTab = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("idWomenRanking");
        if (qMenRankingTab == nullptr)
        {
            qCritical() << "Item idWomenRanking not found";
            return;
        }

        QQuickItem* qSingleRankingTab = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("idSingleRanking");
        if (qMenRankingTab == nullptr)
        {
            qCritical() << "Item idSingleRanking not found";
            return;
        }

        QQuickItem* qChronoListTab = m_pAppEngine->rootObjects().first()->findChild<QQuickItem*>("idChronoScores");
        if (qChronoListTab == nullptr)
        {
            qCritical() << "Item idChronoScores not found";
            return;
        }

        // update only the current selected tab
        if (qMenRankingTab->isEnabled())
        {
            m_pGymMenModel->updateScores();
            m_qSortMProxy->sort(0, Qt::AscendingOrder);
        }

        if (qWomenRankingTab->isEnabled())
        {
            m_pGymWomenModel->updateScores();
            m_qSortWProxy->sort(0, Qt::AscendingOrder);
        }

        if (qSingleRankingTab->isEnabled())
        {
            m_pSingleMModel->updateScores();
            m_qSortSingleMProxy->sort(0, Qt::AscendingOrder);

            m_pSingleWModel->updateScores();
            m_qSortSingleWProxy->sort(0, Qt::AscendingOrder);
        }

        if (qChronoListTab->isEnabled())
        {
            m_pChronoListModel->updateScores();
            // sort not necessary, already sorted in DB view
        }
}

void CoreApplication::onApparatusMChanged(QString p_currentTxt)
{
    if (p_currentTxt != "Attrezzo..")
    {
        // set Title
        QObject* ttiApparatusMen = m_pAppEngine->rootObjects().first()->findChild<QObject*>("singleApparatusMId");
        if (ttiApparatusMen)
        {

            // only exception
            if (p_currentTxt == "Cavallo a maniglie")
                p_currentTxt = "Cavallo a\nmaniglie"; // split into 2 lines

            ttiApparatusMen->setProperty("text", p_currentTxt);

            QString strImageFile = "/images/";
            ApparatusList::EApparatusMen eApparatusM;

            if      (p_currentTxt.compare("Suolo") ==  0) { strImageFile += "Floor.svg"; eApparatusM = ApparatusList::AMFloor; }
            else if (p_currentTxt.compare("Cavallo a\nmaniglie") == 0) { strImageFile += "PommelHorse.svg"; eApparatusM = ApparatusList::AMPommelHorse; }
            else if (p_currentTxt.compare("Anelli") ==  0) { strImageFile += "Rings.svg"; eApparatusM = ApparatusList::AMRings; }
            else if (p_currentTxt.compare("Volteggio") ==  0) { strImageFile += "Vault.svg"; eApparatusM = ApparatusList::AMVault; }
            else if (p_currentTxt.compare("Parallele") ==  0) { strImageFile += "ParallelBars.svg"; eApparatusM = ApparatusList::AMParallelBars; }
            else if (p_currentTxt.compare("Sbarra") ==  0) { strImageFile += "HorizontalBar.svg"; eApparatusM = ApparatusList::AMHorizontalBar; }
            else
            {
                strImageFile += "Empty.svg";
                qCritical() << "Wrong apparatus selected";
                return; // ?
            }

            // set image
            ttiApparatusMen->setProperty("imageSource", strImageFile);

            // Retrieve scores for the selected floor
            m_pSingleMModel->setApparatus(eApparatusM);
            m_pSingleMModel->updateScores();
        }
    }
 }


void CoreApplication::onApparatusWChanged(QString p_currentTxt)
{
    if (p_currentTxt != "Attrezzo..")
    {
        // set Title
        QObject* ttiApparatusWomen = m_pAppEngine->rootObjects().first()->findChild<QObject*>("singleApparatusWId");
        if (ttiApparatusWomen)
        {

            // only exception
            if (p_currentTxt == "Parallele asimmetriche")
                p_currentTxt = "Parallele \nasimmetriche"; // split into 2 lines

            ttiApparatusWomen->setProperty("text", p_currentTxt);

            QString strImageFile = "/images/";
            ApparatusList::EApparatusWomen eApparatusW;

            if      (p_currentTxt.compare("Suolo") ==  0) { strImageFile += "Floor.svg"; eApparatusW = ApparatusList::AWFloor; }
            else if (p_currentTxt.compare("Volteggio") ==  0) { strImageFile += "Vault.svg"; eApparatusW = ApparatusList::AWVault; }
            else if (p_currentTxt.compare("Parallele \nasimmetriche") ==  0) { strImageFile += "UnevenBars.svg"; eApparatusW = ApparatusList::AWUnevenBars; }
            else if (p_currentTxt.compare("Trave") ==  0) { strImageFile += "BalanceBeam.svg"; eApparatusW = ApparatusList::AWBalanceBeam; }
            else
            {
                strImageFile += "Empty.svg";
                qCritical() << "Wrong apparatus selected";
                return; // ?
            }

            // set image
            ttiApparatusWomen->setProperty("imageSource", strImageFile);

            // Retrieve scores for the selected floor
            m_pSingleWModel->setApparatus(eApparatusW);
            m_pSingleWModel->updateScores();
        }
    }
 }
