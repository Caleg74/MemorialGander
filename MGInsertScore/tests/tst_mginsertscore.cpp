#include <QtTest>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QQuickItem>
#include <QQmlComponent>
#include "dbinterface.h"
#include "coreapplication.h"

class InsertScoreTest : public QObject
{
    Q_OBJECT
private slots:
    void listNotifications()
    {
        ComboBoxModel model;
        QCOMPARE(model.count(), 0);
        QSignalSpy listChanges(&model, &ComboBoxModel::comboListChanged);
        QSignalSpy countChanges(&model, &ComboBoxModel::countChanged);
        model.setComboList({"A", "B"});
        QCOMPARE(model.count(), 2);
        QCOMPARE(countChanges.count(), 1);
        model.setComboList({"C", "D"});
        QCOMPARE(listChanges.count(), 2);
        QCOMPARE(countChanges.count(), 1);
        model.setComboList({"C", "D"});
        QCOMPARE(listChanges.count(), 2);
        model.removeElement(-1);
        model.removeElement(2);
        QCOMPARE(model.count(), 2);
        model.removeElement(0);
        model.addElement("E");
        QCOMPARE(model.comboList(), QStringList({"D", "E"}));
        QCOMPARE(countChanges.count(), 3);
    }

    void emptySelection()
    {
        CoreApplication controller;
        controller.setCategories(false, false);
        controller.selectGymnast("");
        QCOMPARE(controller.gymnasts()->count(), 1);
        QCOMPARE(controller.apparatus()->count(), 1);
        QSignalSpy messages(&controller, &CoreApplication::messageRequested);
        QVERIFY(!controller.saveScore("Seleziona ginnasti..", "Attrezzo..", "5", "9", "0", "0", "14", false));
        QCOMPARE(messages.count(), 1);
        QVERIFY(!QSqlDatabase::contains("ConnMG"));
    }

    void invalidScores_data()
    {
        QTest::addColumn<QString>("difficulty");
        QTest::newRow("text") << "abc";
        QTest::newRow("negative") << "-1";
        QTest::newRow("over-limit") << "11";
        QTest::newRow("not-finite") << "nan";
    }

    void invalidScores()
    {
        QFETCH(QString, difficulty);
        SaveScore saver;
        QSignalSpy messages(&saver, &SaveScore::messageRequested);
        QVERIFY(!saver.onSaveScore("Test, Athlete", "Suolo", difficulty, "9", "0", "0", "14", false));
        QCOMPARE(messages.count(), 1);
        QVERIFY(!QSqlDatabase::contains("ConnMG"));
    }

    void qmlLifecycle()
    {
        QTest::failOnWarning();
        CoreApplication controller;
        {
            QQmlApplicationEngine engine;
            engine.setInitialProperties({{"controller", QVariant::fromValue(&controller)}});
            engine.load(QUrl("qrc:/qml/main.qml"));
            QCOMPARE(engine.rootObjects().size(), 1);
            auto *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
            QVERIFY(window);
            QTest::qWait(100);
            auto *save = window->findChild<QObject*>("btnSaveScore");
            QVERIFY(save);
            QVERIFY(!save->property("enabled").toBool());
            auto *settings = window->findChild<QObject*>("popupSettings");
            QVERIFY(settings);
            QVERIFY(QMetaObject::invokeMethod(settings, "open"));
            QTest::qWait(350);
            auto *men = window->findChild<QObject*>("categoryMen");
            auto *women = window->findChild<QObject*>("categoryWomen");
            QVERIFY(men);
            QVERIFY(women);
            men->setProperty("checked", false);
            women->setProperty("checked", false);
            QVERIFY(QMetaObject::invokeMethod(settings, "close"));
            QTest::qWait(350);
            QCOMPARE(controller.gymnasts()->count(), 1);
            QVERIFY(!QSqlDatabase::contains("ConnMG"));
            // Verify the QML Connections handler displays controller errors.
            QVERIFY(!controller.saveScore("", "", "5", "9", "0", "0", "14", false));
            QTest::qWait(50);
            auto *dialog = window->findChild<QObject*>("messageDialog");
            QVERIFY(dialog);
            QVERIFY(dialog->property("visible").toBool());
            window->close();
        }
        QCOMPARE(controller.gymnasts()->count(), 1);
    }

    void apparatusTextEmphasis()
    {
        QTest::failOnWarning();
        QQmlApplicationEngine engine;
        QQmlComponent component(&engine);
        component.setData(R"(
            import QtQuick
            import QtQuick.Controls.Basic
            ApplicationWindow {
                visible: true
                width: 450
                height: 480
                StyleMGComboBox {
                    objectName: "testApparatus"
                    x: 30
                    y: 25
                    width: 360
                    model: ["Attrezzo..", "Suolo", "Cavallo a maniglie", "Anelli",
                            "Volteggio", "Parallele", "Sbarra"]
                    currentIndex: 3
                    emphasizedIndex: 3
                }
            }
        )", QUrl("qrc:/qml/EmphasisTest.qml"));
        QScopedPointer<QObject> root(component.create());
        QVERIFY2(root, qPrintable(component.errorString()));
        auto *window = qobject_cast<QQuickWindow *>(root.data());
        QVERIFY(window);
        auto *combo = window->findChild<QQuickItem *>("testApparatus");
        QVERIFY(combo);
        auto *label = combo->property("contentItem").value<QQuickItem *>();
        QVERIFY(label);
        QCOMPARE(label->property("color").value<QColor>(), QColor("#0000ff"));
        auto *popup = combo->property("popup").value<QObject *>();
        QVERIFY(popup);
        QVERIFY(QMetaObject::invokeMethod(popup, "open"));
        QTest::qWait(200);
        auto *list = popup->property("contentItem").value<QQuickItem *>();
        QVERIFY(list);
        QQuickItem *finalLabel = nullptr;
        QQuickItem *otherLabel = nullptr;
        const auto findLabels = [&](auto &&self, QQuickItem *item) -> void {
            if (item->property("text").toString() == "Anelli"
                && item->property("color").isValid())
                finalLabel = item;
            if (item->property("text").toString() == "Suolo"
                && item->property("color").isValid())
                otherLabel = item;
            for (auto *child : item->childItems())
                self(self, child);
        };
        findLabels(findLabels, list);
        QVERIFY(finalLabel);
        QVERIFY(otherLabel);
        QCOMPARE(finalLabel->property("color").value<QColor>(), QColor("#0000ff"));
        QCOMPARE(otherLabel->property("color").value<QColor>(), QColor("#0a3f60"));
        const auto screenshot = qEnvironmentVariable("MG_TEST_SCREENSHOT");
        if (!screenshot.isEmpty())
            QVERIFY(window->grabWindow().save(screenshot));
        combo->setProperty("currentIndex", 2);
        QCOMPARE(label->property("color").value<QColor>(), QColor("#0a3f60"));
        // The final apparatus stays blue even when another item is selected.
        QCOMPARE(finalLabel->property("color").value<QColor>(), QColor("#0000ff"));
        combo->setProperty("emphasizedIndex", -1);
        QCOMPARE(finalLabel->property("color").value<QColor>(), QColor("#0a3f60"));
        combo->setProperty("currentIndex", 3);
        QCOMPARE(label->property("color").value<QColor>(), QColor("#0a3f60"));
        window->close();
    }

    void liveDatabaseStartup()
    {
        if (!qEnvironmentVariableIsSet("MG_TEST_LIVE_DB"))
            QSKIP("Set MG_TEST_LIVE_DB=1 for the read-only local ODBC startup check");
        QTest::failOnWarning();
        CoreApplication controller;
        QVERIFY(controller.initialize());
        QQmlApplicationEngine engine;
        engine.setInitialProperties({{"controller", QVariant::fromValue(&controller)}});
        engine.load(QUrl("qrc:/qml/main.qml"));
        QCOMPARE(engine.rootObjects().size(), 1);
        QTest::qWait(200);
        auto *window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
        QVERIFY(window);
        auto *gymnasts = window->findChild<QObject *>("cbbGymnastSelection");
        auto *apparatus = window->findChild<QQuickItem *>("apparatusSelection");
        QVERIFY(gymnasts);
        QVERIFY(apparatus);
        auto *database = dbInterface::Instance();
        const int eventId = database->getCurrentEventId();
        const auto names = controller.gymnasts()->comboList();
        QVERIFY(names.size() > 1);
        int assignments = 0;
        for (int i = 1; i < names.size(); ++i) {
            const auto parts = names.at(i).split(',');
            QString firstName = parts.at(0).trimmed();
            QString lastName = parts.at(1).trimmed();
            const int athleteId = database->getGymnastId(firstName, lastName);
            QSqlQuery query(QSqlDatabase::database("ConnMG"));
            query.prepare("SELECT a.name_it FROM sport_event_subscriptions s "
                          "LEFT JOIN apparatus a ON a.id = s.final_apparatus_id "
                          "WHERE s.athlete_id = ? AND s.sport_event_id = ?");
            query.addBindValue(athleteId);
            query.addBindValue(eventId);
            QVERIFY(query.exec());
            QVERIFY(query.next());
            gymnasts->setProperty("currentIndex", i);
            const QString finalName = query.value(0).toString();
            const int expected = finalName.isEmpty()
                ? -1 : controller.apparatus()->comboList().indexOf(finalName);
            QCOMPARE(controller.finalApparatusIndex(), expected);
            QCOMPARE(apparatus->property("emphasizedIndex").toInt(), expected);
            if (expected > 0) {
                apparatus->setProperty("currentIndex", expected);
                auto *label = apparatus->property("contentItem").value<QQuickItem *>();
                QVERIFY(label);
                QCOMPARE(label->property("text").toString(), finalName);
                QCOMPARE(label->property("color").value<QColor>(), QColor("#0000ff"));
                const auto screenshot = qEnvironmentVariable("MG_TEST_SCREENSHOT");
                if (assignments == 0 && !screenshot.isEmpty()) {
                    auto *popup = apparatus->property("popup").value<QObject *>();
                    QVERIFY(popup);
                    QVERIFY(QMetaObject::invokeMethod(popup, "open"));
                    QTest::qWait(200);
                    QVERIFY(window->grabWindow().save(screenshot + ".live.png"));
                    QVERIFY(QMetaObject::invokeMethod(popup, "close"));
                }
                ++assignments;
            }
        }
        qInfo() << "Verified gymnasts:" << names.size() - 1
                << "with final apparatus:" << assignments;
        gymnasts->setProperty("currentIndex", 0);
        QCOMPARE(controller.finalApparatusIndex(), -1);
        QCOMPARE(apparatus->property("emphasizedIndex").toInt(), -1);
        window->close();
    }
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");
    InsertScoreTest test;
    return QTest::qExec(&test, argc, argv);
}
#include "tst_mginsertscore.moc"
