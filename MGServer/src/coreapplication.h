#ifndef COREAPPLICATION_H
#define COREAPPLICATION_H

#include <QList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlregistration.h>
#include "gymnastsortfilterproxymodel.h"
#include "comboboxmodel.h"
#include "createpdf.h"

class CoreApplication : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("MGServer owns the application controller")
    Q_PROPERTY(QList<int> editions READ editions CONSTANT)
    Q_PROPERTY(int currentEvent READ getCurrentEvent NOTIFY currentEventChanged)

public:
    explicit CoreApplication(QObject *parent = nullptr);

    [[nodiscard]] QList<int> editions() const;
    [[nodiscard]] int getCurrentEvent() const;
    Q_INVOKABLE bool selectEvent(int event);

    /** Initializes the core application components */
    void Init(QQmlApplicationEngine &p_qEngine);

    /** Connectes all UI signals */
    void Connect();

signals:
    void currentEventChanged();

public slots:
    void onGymnastChanged(QString p_currentTxt);

private:
    QQmlApplicationEngine* m_pAppEngine = nullptr;

    GymnastSortFilterProxyModel* m_qSortProxyGymnast = nullptr;
    GymnastSortFilterProxyModel* m_qSortProxySelectedGymnast = nullptr;

    ComboBoxModel m_pApparatusCbbModel;

    CreatePdf m_cPdf;
};

#endif // COREAPPLICATION_H
