#include "coreapplication.h"
#include "apparatuslist.h"
#include "dbinterface.h"
#include <QDebug>

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
    if (parts.size() < 2 || parts[0].trimmed().isEmpty() || parts[1].trimmed().isEmpty()) {
        m_apparatus.setComboList({QStringLiteral("Attrezzo..")});
        return;
    }
    QString firstName = parts[0].trimmed();
    QString lastName = parts[1].trimmed();
    auto *database = dbInterface::Instance();
    const int athleteId = database->getGymnastId(firstName, lastName);
    ApparatusList::Instance()->FillComboList(&m_apparatus, database->getGender(athleteId));
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
