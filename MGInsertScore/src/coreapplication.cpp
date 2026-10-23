#include "coreapplication.h"
#include "apparatuslist.h"
#include "dbinterface.h"
#include <QDebug>
#include <cmath>

CoreApplication::CoreApplication(QObject *parent) : QObject(parent)
{
    m_gymnasts.setComboList({QStringLiteral("Seleziona ginnasti..")});
    m_apparatus.setComboList({QStringLiteral("Attrezzo..")});
    connect(&m_saveScore, &SaveScore::messageRequested,
            this, &CoreApplication::messageRequested);
}

bool CoreApplication::initialize()
{
    auto *database = dbInterface::Instance();
    if (!database->loadDriver() || !database->initDb(false)) {
        qCritical() << "Database initialization failed:" << database->getLastError();
        return false;
    }
    ApparatusList::Instance()->load();
    setCategories(true, true);
    return true;
}

void CoreApplication::selectGymnast(const QString &name)
{
    const auto parts = name.split(',');
    int finalIndex = -1;
    if (parts.size() < 2 || parts[0].trimmed().isEmpty() || parts[1].trimmed().isEmpty()) {
        m_apparatus.setComboList({QStringLiteral("Attrezzo..")});
    } else {
        QString firstName = parts[0].trimmed();
        QString lastName = parts[1].trimmed();
        auto *database = dbInterface::Instance();
        const int athleteId = database->getGymnastId(firstName, lastName);
        auto *apparatusList = ApparatusList::Instance();
        apparatusList->FillComboList(&m_apparatus, database->getGender(athleteId));
        const int finalId = database->getFinalApparatusId(athleteId, database->getCurrentEventId());
        if (finalId > 0) {
            const int index = m_apparatus.comboList().indexOf(apparatusList->apparatusName(finalId));
            if (index > 0)
                finalIndex = index;
        }
    }
    if (m_finalApparatusIndex != finalIndex) {
        m_finalApparatusIndex = finalIndex;
        emit finalApparatusIndexChanged();
    }
}

QVariantMap CoreApplication::existingScore(const QString &gymnast, const QString &apparatus)
{
    const QStringList parts = gymnast.split(',');
    if (parts.size() < 2 || parts.at(0).trimmed().isEmpty() || parts.at(1).trimmed().isEmpty())
        return {{QStringLiteral("found"), false}};

    QString firstName = parts.at(0).trimmed();
    QString lastName = parts.at(1).trimmed();
    QString apparatusName = apparatus;
    auto *database = dbInterface::Instance();
    const int athleteId = database->getGymnastId(firstName, lastName);
    if (athleteId <= 0)
        return {{QStringLiteral("found"), false}};

    QString gender = database->getGender(athleteId);
    const int apparatusId = database->getApparatusId(apparatusName, gender);
    const int eventId = database->getCurrentEventId();
    if (apparatusId <= 0 || eventId <= 0 || !database->isScorePresent(eventId, athleteId, apparatusId))
        return {{QStringLiteral("found"), false}};

    const AllScores scores = database->getAllScores(athleteId, apparatusId);
    return {
        {QStringLiteral("found"), true},
        {QStringLiteral("difficulty"), QString::number(scores.DifficultyScore, 'f', 3)},
        {QStringLiteral("penalty"), QString::number(std::abs(scores.PenaltyScore), 'f', 3)}
    };
}

void CoreApplication::setCategories(bool men, bool women)
{
    QStringList names;
    if (men || women)
        dbInterface::Instance()->getEventSelectedGymnastList(&names, men, women);
    else
        names << QStringLiteral("Seleziona ginnasti..");
    m_gymnasts.setComboList(names);
}

bool CoreApplication::saveScore(const QString &gymnast, const QString &apparatus,
                               const QString &difficulty, const QString &execution,
                               const QString &penalty, const QString &finalScore, bool finalApparatus)
{
    if (m_gymnasts.comboList().indexOf(gymnast) <= 0 || m_apparatus.comboList().indexOf(apparatus) <= 0) {
        emit messageRequested(tr("Selezione incompleta"), tr("Seleziona un ginnasta e un attrezzo."));
        return false;
    }
    return m_saveScore.onSaveScore(gymnast, apparatus, difficulty, execution, penalty, finalScore, finalApparatus);
}
