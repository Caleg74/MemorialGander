#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <cstdlib>
#include "coreapplication.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("MGInsertScore"));
    QQuickStyle::setStyle(QStringLiteral("Basic"));

    // The controller and its models must outlive the QML engine.
    CoreApplication controller;
    if (!controller.initialize())
        return EXIT_FAILURE;
    QQmlApplicationEngine engine;
    engine.setInitialProperties({{QStringLiteral("controller"), QVariant::fromValue(&controller)}});
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return EXIT_FAILURE;
    return app.exec();
}
