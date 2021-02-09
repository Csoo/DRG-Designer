#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include "../icd-project/backendDatabase/database.h"
#include <QSqlQuery>
#include <QThread>
#include "qmlmanager.h"

class DRG;
class ICD11;
class TreeItem;

class ConnectionThread: public QObject {
    Q_OBJECT
    bool m_stop;
    bool m_pause;
public:
    explicit ConnectionThread(QObject *parent = nullptr);
public slots:
    void connectToDatabase(Database *db);
    void interruptConnection(int state = 1);
signals:
    void connectionReady(bool success, const QString &error);
};

class Item;

class Repository : public QObject
{
    Q_OBJECT

    Database *m_db;
    QMLManager *m_qml;
    QThread dbConnectionThread;
    friend class ConnectionThread;
public:
    explicit Repository(QMLManager *qml, QObject *parent = nullptr);
    ~Repository();

public slots:
    void authentication(const QString &user, const QString &pw);
    void loadDrgBrowser(int drgId);
    void loadDrgList();
    void loadChapterDrg(int chapterId);
    void loadIcd(int conceptType, int icdId);
    void loadPostCoordTree(int icdId, int conceptType);
    void killConnectionThread(int state);
    void searchIcd11(const QString &icdText);
    void loadRecommendation(int id);
    void loadICDDetails(int icdId, int conceptType);
    void loadChildren(unsigned int id, int type, const QModelIndex &parent);
    void login(const QString &userName, const QString &password);
    void logout(int userId);

signals:
    void startConnectToDatabase(Database *db);
    void connectionReady(bool success, const QString &error);
    void loadDrgListReady(const QList<Item> &drgs);
    void drgBrowserReady(TreeItem *rootItem);
    void drgChapterReady(int chapterId, const QList<DRG*> &drgList);
    void icdEntityReady(const QModelIndex &icdIndex, const ICD11 &icd);
    void postCoordReady(int numberOfAxis, ICD11 *root);
    void ICDDetailsReady(ICD11 *icd);
    void quitDbThread();
    void interruptThread(int state);
    void searchIcd11Ready(const QList<Item> &);
    void recommendationReady(bool noRecommendation, ICD11 *recommendation);
    void loadChildrenReady(const QList<ICD11*>, const QModelIndex &parent);
    void loginReady(int userId);
};

#endif // REPOSITORY_H
