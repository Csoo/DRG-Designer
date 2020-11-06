#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include "../icd-project/backendDatabase/database.h"

class Item {
public:
    Item(int id, const QString &code, const QString &title);

    QString getCode() const;

    QString getTitle() const;

    int getId() const;

private:
    int id;
    QString code;
    QString title;
};

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum itemRoles {
        IdRole = Qt::UserRole + 1,
        CodeRole,
        TitleRole
    };
    explicit ListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setDb(Database *value);

    Q_INVOKABLE void loadDrgs();
    Q_INVOKABLE void loadIcd11(const QString &icd);
    Q_INVOKABLE void clear();

private:
    QList<Item> items;

    Database  *db;
};

#endif // LISTMODEL_H
