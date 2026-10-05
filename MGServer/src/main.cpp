#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "CoreApplication.h"
#include "countrylist.h"
#include "gymnastselectionlist.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");
    CoreApplication cCoreApp;
    QQmlApplicationEngine engine;
    cCoreApp.Init(engine);

    CountryList cCountryList;
    cCountryList.Init(engine);

    GymnastSelectionList::Instance()->Init(engine);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return 1;

    cCoreApp.Connect();   // connect all signals from the UI

//    QObject *topLevel = engine.rootObjects().value(0);
//    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
//    window->show();

    return app.exec();
}

