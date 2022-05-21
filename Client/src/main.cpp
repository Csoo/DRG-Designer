#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "QuickQanava.h"
#include "treemodel.h"
#include "drgchapter.h"
#include "listmodel.h"
#include "treeitem.h"
#include "icd11.h"
#include "drg.h"
#include "session.h"
#include "enum.hpp"
#include "repository.h"
#include "../icd-project/backendDatabase/database.h"
#include <QThread>
#include "qmlmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Universal");

    QThread *dbThread = new QThread();
    QMLManager qml(dbThread);
    //Database *db = new Database;
    Repository db1(&qml);
    Session session(&db1);
    db1.moveToThread(dbThread);
    dbThread->start();

    TreeModel treeModel(&db1, dbThread);
    TreeModel postCoordModel(&db1, dbThread);
    ICD11 selectedICD(&db1, dbThread);
    selectedICD.setModel(&treeModel);
    DRG selectedDRG;
    selectedDRG.setModel(&treeModel);
    //treeModel.setDb(db);
    //postCoordModel.setDb(db);
    ListModel listModel(&db1, dbThread);
    //listModel.setDb(db);

    qRegisterMetaType<QList<DRG*>>("<QList<DRG*>>");
    qRegisterMetaType<QList<ICD11*>>("<QList<ICD*>>");
    QObject::connect(&treeModel, &TreeModel::loadDrgBrowser, &db1, &Repository::loadDrgBrowser);
    QObject::connect(&db1, &Repository::drgBrowserReady, &treeModel, &TreeModel::drgBrowserResult);
    QObject::connect(&treeModel, &TreeModel::loadChapterDrg, &db1, &Repository::loadChapterDrg);
    QObject::connect(&db1, &Repository::drgChapterReady, &treeModel, &TreeModel::drgChapterResult);
    QObject::connect(&treeModel, &TreeModel::loadIcdEntity, &db1, &Repository::loadIcd);
    QObject::connect(&db1, &Repository::icdEntityReady, &treeModel, &TreeModel::icdEntityResult);
    QObject::connect(&postCoordModel, &TreeModel::loadPostCoordTree, &db1, &Repository::loadPostCoordTree);
    QObject::connect(&db1, &Repository::postCoordReady, &postCoordModel, &TreeModel::postCoordResult);
    QObject::connect(&postCoordModel, &TreeModel::loadItemChildren, &db1, &Repository::loadChildren);
    QObject::connect(&db1, &Repository::loadChildrenReady, &postCoordModel, &TreeModel::loadChildrenResult);
    QObject::connect(&listModel, &ListModel::searchIcd11, &db1, &Repository::searchIcd11);
    QObject::connect(&db1, &Repository::searchIcd11Ready, &listModel, &ListModel::searchIcd11Result);

    QQmlApplicationEngine engine;
    engine.addPluginPath(QStringLiteral("QuickQanava/src"));
    QuickQanava::initialize(&engine);

    engine.rootContext()->setContextProperty("session", &session);
    engine.rootContext()->setContextProperty("listModel", &listModel);
    engine.rootContext()->setContextProperty("treeModel", &treeModel);
    engine.rootContext()->setContextProperty("postCoordModel", &postCoordModel);
    engine.rootContext()->setContextProperty("icd", &selectedICD);
    engine.rootContext()->setContextProperty("drg", &selectedDRG);
    engine.rootContext()->setContextProperty("qmlManager", &qml);
    qmlRegisterType<Type>("TypeEnum", 1, 0, "Type");
    qmlRegisterType<qan::Graph>("MyGraph",1,0,"CustomGraph");

    const QUrl url(QStringLiteral("qrc:/app.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
