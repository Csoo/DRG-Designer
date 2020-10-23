#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "QuickQanava.h"
#include "drgbrowsermodel.h"
#include "drgchapter.h"
#include "listmodel.h"
#include "icd11.h"
#include "drg.h"
#include "enum.hpp"
#include "../icd-project/backendDatabase/database.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Universal");
    Database *db = new Database;
    DRGBrowserModel treeModel;
    DRGBrowserModel postCoordModel;
    ICD11 selectedICD(&treeModel);
    DRG selectedDRG(&treeModel);
    treeModel.setDb(db);
    postCoordModel.setDb(db);
    ListModel listModel;
    listModel.setDb(db);

    QQmlApplicationEngine engine;
    engine.addPluginPath(QStringLiteral("QuickQanava/src"));
    QuickQanava::initialize(&engine);
    engine.rootContext()->setContextProperty("listModel", &listModel);
    engine.rootContext()->setContextProperty("treeModel", &treeModel);
    engine.rootContext()->setContextProperty("postCoordModel", &postCoordModel);
    engine.rootContext()->setContextProperty("icd", &selectedICD);
    engine.rootContext()->setContextProperty("drg", &selectedDRG);
    qmlRegisterType<Type>("TypeEnum", 1, 0, "Type");
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
