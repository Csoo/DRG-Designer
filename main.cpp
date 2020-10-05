#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "QuickQanava.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Universal");

    QQmlApplicationEngine engine;
    engine.addPluginPath(QStringLiteral("QuickQanava/src"));
    QuickQanava::initialize(&engine);
    qmlRegisterType<qan::Graph>("MyGraph",1,0,"CustomGraph");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
