#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include "../icd-project/backendDatabase/database.h"
#include "repository.h"
#include <QThread>

class Item {
public:
    Item(int id, const QString &code, const QString &title, int conceptType = -1);

    QString getCode() const;
    QString getTitle() const;
    int getId() const;
    int getConceptType() const;

private:
    int id;
    int conceptType;
    QString code;
    QString title;
};

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum itemRoles {
        IdRole = Qt::UserRole + 1,
        ConceptType,
        CodeRole,
        TitleRole
    };
    explicit ListModel(Repository *db, QThread *dbThread, QObject *parent = nullptr);
    ~ListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    //void setDb(Database *value);

    Q_INVOKABLE void loadDrgs();
    Q_INVOKABLE void loadIcd11(const QString &icd);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void searchIcd(const QString &icdText);

private:
    QList<Item> m_items;

    Repository *db1;
    QThread *dbThread;

public slots:
    void listResult(QList<Item> items);
    void searchIcd11Result(QList<Item> items);

signals:
    void startLoadDrgs();
    void listReady();
    void searchIcd11(const QString &icdText);
};

#endif // LISTMODEL_H
