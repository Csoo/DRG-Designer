#include "qmlmanager.h"
#include <QtDebug>

QMLManager::QMLManager(QThread *dbThread, QObject *parent) :
    QObject(parent),
    dbThread(dbThread)
{

}

void QMLManager::connectToDatabase()
{
    emit startConnectToDatabase();
    qDebug() << "QMLManager";
}

void QMLManager::quitDbThread()
{
    dbThread->quit();
}

void QMLManager::connectionResult(bool success, const QString &error)
{
    emit connectionReady(success, error);
}

void QMLManager::drgBrowserResult()
{
    qDebug() << "drg ready for qml";
    emit drgBrowserReady();
}
