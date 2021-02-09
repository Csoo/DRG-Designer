#include "repository.h"
#include "listmodel.h"
#include "treeitem.h"
#include "icd11.h"
#include "drgchapter.h"
#include "drg.h"
#include "icd11.h"
#include "enum.hpp"

ConnectionThread::ConnectionThread(QObject *parent):
    QObject(parent),
    m_stop(false)
{
    qRegisterMetaType<Database*>("Database*");
}

void ConnectionThread::connectToDatabase(Database *db)
{
    if (m_stop) {
        emit connectionReady(true, "QUIT");
        return;
    }
    QString error = "";
    bool success = true;
    if (!m_pause) {
        if (db->testConnection(&error))
            success = true;
        else {
            qInfo("Connecting to database...");
            success = db->connectDatabase(&error);
            success ? qInfo("Database connection alive.") :  qInfo("Connection error: %s", qPrintable(error));
        }
    }

    emit connectionReady(success, error);
    QThread::sleep(13);
    QMetaObject::invokeMethod(this, "connectToDatabase", Qt::QueuedConnection, Q_ARG(Database*, db));
}

void ConnectionThread::interruptConnection(int state)
{
    switch (state) {
        case Type::STOP: {
            m_stop = true;
            m_pause = false;
            break;
        }
        case Type::PAUSE: {
            m_stop = false;
            m_pause = true;
            break;
        }
        default: {
            m_stop = false;
            m_pause = false;
        }
    }
}

Repository::Repository(QMLManager *qml, QObject *parent) : QObject(parent),
    m_db(new Database),
    m_qml(qml)
{
    ConnectionThread *connectionThread = new ConnectionThread;
    connectionThread->moveToThread(&dbConnectionThread);
    connect(m_qml, &QMLManager::stop, this, &Repository::killConnectionThread);
    connect(this, &Repository::interruptThread, connectionThread, &ConnectionThread::interruptConnection);
    connect(&dbConnectionThread, &QThread::finished, connectionThread, &QObject::deleteLater);
    connect(this, &Repository::startConnectToDatabase, connectionThread, &ConnectionThread::connectToDatabase);
    connect(connectionThread, &ConnectionThread::connectionReady, m_qml, &QMLManager::connectionResult);

    connect(this, &Repository::drgBrowserReady, m_qml, &QMLManager::drgBrowserResult);
    connect(this, &Repository::postCoordReady, m_qml, &QMLManager::postCoordResult);
    connect(this, &Repository::searchIcd11Ready, m_qml, &QMLManager::searchIcdResult);
    
    connect(this, &Repository::quitDbThread, m_qml, &QMLManager::quitDbThread);

    dbConnectionThread.start();
    emit startConnectToDatabase(m_db);
}

Repository::~Repository()
{
    qDebug() << "~Repo";
    m_db->closeDatabase();
    delete m_db;/*
    dbConnectionThread.quit();
    dbConnectionThread.wait();*/
    emit quitDbThread();
}

void Repository::authentication(const QString &user, const QString &pw)
{

}

void Repository::loadDrgBrowser(int drgId)
{

    TreeItem *rootItem = new TreeItem;
    qInfo() << "Request from database: load browser from DRG id:" << drgId;
    QSqlQuery query = m_db->listDrgChapters(drgId);
    QList<unsigned int> chapterIds;
    while (query.next()) {
        DRGChapter *chapter = new DRGChapter(query.value(0).toInt(), query.value(2).toString(),
                                             query.value(3).toString());
        rootItem->appendChild(chapter);
    }

    for (auto chapter : rootItem->getChildItems()) {
        QSqlQuery drgQuery = m_db->listOrderedDrgBno(chapter->getId());
        unsigned int currentDrg = -1, currentType = -1;
        int typeIdx = 0, drgIdx = -1;
        while (drgQuery.next()) {
            DRG *drg = new DRG(drgQuery.value(0).toInt(), drgQuery.value(1).toString(), drgQuery.value(2).toString());
            TreeItem *type = new TreeItem(Type::DRG_TYPE, drgQuery.value(3).toInt(), drgQuery.value(4).toString());
            ICD11 *icd = new ICD11(drgQuery.value(5).toInt(), -1, drgQuery.value(6).toString(), drgQuery.value(7).toString(), drgQuery.value(8).toInt());
            if (currentDrg != drg->getId()) {
                typeIdx = 0;
                drgIdx++;
                currentDrg = drgQuery.value(0).toInt();
                currentType = drgQuery.value(3).toInt();
                chapter->appendChild(drg);
                if (!drgQuery.value(3).isNull())
                    chapter->child(drgIdx)->appendChild(type);
            } else {
                if (currentType != type->getId()) {
                    typeIdx++;
                    currentType = drgQuery.value(3).toInt();
                    if (!drgQuery.value(3).isNull())
                        chapter->child(drgIdx)->appendChild(type);
                }
            }
            if (!drgQuery.value(5).isNull())
                chapter->child(drgIdx)->child(typeIdx)->appendChild(icd);
        }
    }

    emit drgBrowserReady(rootItem);
}

void Repository::loadDrgList()
{
    QList<Item> items;

    items.clear();
    items.append(Item(0, "", "Létrehozott HBCs hierarchiák..."));
    qInfo("Request from database: list all DRG.");
    QSqlQuery query = m_db->listDrgs();
    while (query.next()) {
        Item newItem(query.value(0).toInt(), query.value(2).toString(), query.value(1).toString());
        items.append(newItem);
    }
    emit loadDrgListReady(items);
}

void Repository::loadChapterDrg(int chapterId)
{
    QList<DRG*> drgList;
    QSqlQuery query = m_db->listDrgEntities(chapterId);
    while (query.next()) {
        DRG *drg = new DRG(query.value(0).toInt());
        drg->setAttributes(query.value(6).toInt(),
                             query.value(7).toInt(),
                             query.value(8).toInt(),
                             query.value(9).toFloat(),
                             query.value(5).toString());
        drgList.append(drg);
    }

    emit drgChapterReady(chapterId, drgList);
}

void Repository::loadIcd(int conceptType, int icdId)
{

}

void Repository::loadPostCoordTree(int icdId, int conceptType)
{
    ICD11 *root = new ICD11;

    int currentAxisId = -1;
    int axisIdx = -1;
    qInfo() << "Request from database: load post-coordinations for icd11:" << icdId << "," << conceptType;
    QSqlQuery postCoordQuery = m_db->listPostCoordinations(icdId, conceptType);

    while (postCoordQuery.next()) {
        TreeItem *axis = new TreeItem(Type::AXIS, postCoordQuery.value(1).toInt(), postCoordQuery.value(2).toString(), "");
        ICD11 *icd = new ICD11(postCoordQuery.value(5).toInt(),
                               postCoordQuery.value(6).toInt(),
                               (postCoordQuery.value(7).isNull() || postCoordQuery.value(7).toString().length() == 2) ? postCoordQuery.value(10).toString() : postCoordQuery.value(7).toString(),
                               (postCoordQuery.value(7).isNull() || postCoordQuery.value(7).toString().length() == 2) ? "" : postCoordQuery.value(10).toString());
        if (postCoordQuery.value(11).toInt() != 0)
            icd->appendChild(new TreeItem());
        if (currentAxisId != postCoordQuery.value(1).toInt()) {
            currentAxisId = postCoordQuery.value(1).toInt();
            axisIdx++;
            axis->appendChild(icd);
            root->appendChild(axis);
        } else {
            root->child(axisIdx)->appendChild(icd);
        }
    }

    emit postCoordReady(root->childCount(), root);
}

void Repository::killConnectionThread(int state)
{
    if (state == 0) {
        dbConnectionThread.terminate();
        dbConnectionThread.wait();
    } else {
        emit interruptThread(state);
    }
}

void Repository::searchIcd11(const QString &icdText)
{
    QList<Item> searchResult;
    qInfo() << "Request from database: search for icd by text:" << icdText;
    QSqlQuery query = m_db->searchIcd11(icdText, true);
    while (query.next()) {
        searchResult.append(Item(query.value(0).toInt(), query.value(2).toString(), query.value(3).toString(), query.value(1).toInt()));
    }
    emit searchIcd11Ready(searchResult);
}

void Repository::loadRecommendation(int id)
{
    ICD11 *icd = new ICD11;
    bool noRecommendation = false;
    qInfo() << "Request from database: WHO suggestions for ICD-10:" << id;
    QSqlQuery query = m_db->listMappingSuggestionToBno10(id);
    if (query.next()) {
        icd->setId(query.value(1).toInt());
        icd->setConceptType(query.value(2).toInt());
        icd->setCode(query.value(3).toString());
        icd->setTitle(query.value(6).toString());
    } else {
        noRecommendation = true;
    }
    while (query.next()) {
        icd->addPostCoordination(new ICD11(query.value(1).toInt(),
                                          query.value(2).toInt(),
                                          query.value(3).toString(),
                                          query.value(6).toString()));
    }
    emit recommendationReady(noRecommendation, icd);
}

void Repository::loadICDDetails(int icdId, int conceptType)
{
    ICD11 *icd = new ICD11();
    qInfo() << "Request from database: load details for ICD-11:" << icdId << conceptType;
    QSqlQuery query = m_db->listIcd11Linea(icdId, conceptType);
    while (query.next()) {
        icd->setDescription(query.value(17).toString());
    }
    //icd->setDescription("Occaecati eum sunt quia ipsam facere aut nobis laudantium. Inventore et amet tempora cumque molestiae culpa beatae laborum. Doloremque nisi nostrum quasi. Asperiores ratione occaecati enim omnis voluptatem fugit totam. Labore itaque sequi et. Hic est soluta quisquam quasi facilis consequuntur libero.");
    emit ICDDetailsReady(icd);
}

void Repository::loadChildren(unsigned int id, int type, const QModelIndex &parent)
{
    QList<ICD11*> icdList;
    qInfo() << "Request from database: load children items to ICD-11:" << id;
    QSqlQuery query = m_db->getPostcoordinationChild(id, type);
    while (query.next()) {
        ICD11 *icd = new ICD11(query.value(0).toInt(),
                              query.value(1).toInt(),
                              query.value(2).isNull() ? query.value(5).toString() : query.value(2).toString(),
                              query.value(2).isNull() ? "" : query.value(5).toString());
        if (query.value(8).toInt() != 0)
            icd->appendChild(new TreeItem());
        icdList.append(icd);
    }
    emit loadChildrenReady(icdList, parent);
}

void Repository::login(const QString &userName, const QString &password)
{
    qInfo() << "Login request, userName:" << userName;
    int userId = m_db->login(userName, password);
    if (userId != -1) {
        m_db->loginLog(userId);
    }

    emit loginReady(userId);
}

void Repository::logout(int userId)
{
    m_db->logoutLog(userId);
    qInfo() << "Logout user:" << userId;
}
