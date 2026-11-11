#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "coreapplication.h"
#include "countrylist.h"
#include "gymnastselectionlist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("MGServer"));

    QQuickStyle::setStyle("Basic");
    CoreApplication cCoreApp;
    QQmlApplicationEngine engine;
    cCoreApp.Init(engine);

    CountryList cCountryList;
    cCountryList.Init(engine);

    GymnastSelectionList::Instance()->Init(engine);

    engine.setInitialProperties({
        {QStringLiteral("coreApplication"), QVariant::fromValue(&cCoreApp)}
    });
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return 1;

    cCoreApp.Connect();   // connect all signals from the UI

//    QObject *topLevel = engine.rootObjects().value(0);
//    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
//    window->show();

    return app.exec();
}

