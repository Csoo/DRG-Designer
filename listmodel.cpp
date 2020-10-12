#include "listmodel.h"

Item::Item(int id, const QString &code, const QString &title):
    id(id),
    code(code),
    title(title)
{

}

QString Item::getCode() const
{
    return code;
}

QString Item::getTitle() const
{
    return title;
}

int Item::getId() const
{
    return id;
}

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return items.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{   
    if (!index.isValid())
        return QVariant();
    if (role != CodeRole && role != TitleRole && role != IdRole)
        return QVariant();

    switch (role) {
        case IdRole: return items.at(index.row()).getId();
        case CodeRole: return items.at(index.row()).getCode();
        case TitleRole: return items.at(index.row()).getTitle();
    }
    return QVariant();
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[CodeRole] = "code";
    names[TitleRole] = "title";
    return names;
}

void ListModel::setDb(Database *value)
{
    db = value;
}

void ListModel::loadDrgs()
{
    beginResetModel();

    items.clear();
    items.append(Item(0, "", "Létrehozott HBCs hierarchiák..."));
    QSqlQuery query = db->listDrgs();
    while (query.next()) {
        Item newItem(query.value(0).toInt(), query.value(2).toString(), query.value(1).toString());
        items.append(newItem);
    }
    endResetModel();
}

