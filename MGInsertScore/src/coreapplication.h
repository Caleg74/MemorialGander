#ifndef COREAPPLICATION_H
#define COREAPPLICATION_H
#include <QObject>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>
#include "comboboxmodel.h"
#include "savescore.h"

class CoreApplication : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ScoreController)
    QML_UNCREATABLE("The application owns the score controller")
    Q_PROPERTY(ComboBoxModel* gymnasts READ gymnasts CONSTANT)
    Q_PROPERTY(ComboBoxModel* apparatus READ apparatus CONSTANT)
    Q_PROPERTY(int finalApparatusIndex READ finalApparatusIndex NOTIFY finalApparatusIndexChanged)
public:
    explicit CoreApplication(QObject *parent = nullptr);
    bool initialize();
    ComboBoxModel* gymnasts() { return &m_gymnasts; }
    ComboBoxModel* apparatus() { return &m_apparatus; }
    int finalApparatusIndex() const { return m_finalApparatusIndex; }
public slots:
    void selectGymnast(const QString &name);
    Q_INVOKABLE QVariantMap existingScore(const QString &gymnast, const QString &apparatus);
    void setCategories(bool men, bool women);
    bool saveScore(const QString &gymnast, const QString &apparatus,
                   const QString &difficulty, const QString &execution,
                   const QString &penalty, const QString &finalScore, bool finalApparatus);
signals:
    void finalApparatusIndexChanged();
    void messageRequested(const QString &title, const QString &text);
private:
    ComboBoxModel m_gymnasts;
    ComboBoxModel m_apparatus;
    SaveScore m_saveScore;
    int m_finalApparatusIndex = -1;
};
#endif
