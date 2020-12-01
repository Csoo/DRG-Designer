#ifndef QMLMANAGER_H
#define QMLMANAGER_H

#include <QObject>
#include <QThread>

class QMLManager : public QObject
{
    Q_OBJECT

    QThread *dbThread;
public:
    explicit QMLManager(QThread *dbThread, QObject *parent = nullptr);

    Q_INVOKABLE void connectToDatabase();
public slots:
    void connectionResult(bool success, const QString &error);
    void drgBrowserResult();
    void stopConnection(int state) { emit stop(state); };
    void quitDbThread();
    void searchIcdResult() { emit searchReady(); };
    void postCoordResult() { emit postCoordTreeReady(); };

signals:
    void connectionReady(bool success, const QString &error);
    void drgBrowserReady();
    void startConnectToDatabase();
    void searchReady();
    void stop(int state);
    void postCoordTreeReady();

};

#endif // QMLMANAGER_H
