#include "listmodel.h"

Item::Item(int id, const QString &code, const QString &title, int conceptType):
    id(id),
    conceptType(conceptType),
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

int Item::getConceptType() const
{
    return conceptType;
}

ListModel::ListModel(Repository *db, QThread *dbThread, QObject *parent):
    QAbstractListModel(parent),
    db1(db),
    dbThread(dbThread)
{
    qRegisterMetaType<QList<Item>>("QList<Item>");

    connect(dbThread, &QThread::finished, db1, &QObject::deleteLater);
    connect(this, &ListModel::startLoadDrgs, db1, &Repository::loadDrgList);
    connect(db1, &Repository::loadDrgListReady, this, &ListModel::listResult);
}

ListModel::~ListModel()
{
    qDebug() << "~ListModel";
}

int ListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_items.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{   
    if (!index.isValid())
        return QVariant();
    if (role != CodeRole && role != TitleRole && role != IdRole && role != ConceptType)
        return QVariant();

    switch (role) {
        case IdRole: return m_items.at(index.row()).getId();
        case ConceptType: return m_items.at(index.row()).getConceptType();
        case CodeRole: return m_items.at(index.row()).getCode();
        case TitleRole: return m_items.at(index.row()).getTitle();
    }
    return QVariant();
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[ConceptType] = "conceptType";
    names[CodeRole] = "code";
    names[TitleRole] = "title";
    return names;
}

//void ListModel::setDb(Database *value)
//{
//    db = value;
//}

void ListModel::loadIcd11(const QString &icd)
{
//    beginResetModel();
//    m_items.clear();
//    QSqlQuery query = db->searchIcd11(icd, true);
//    qDebug() << "icd11 search";
//    while (query.next()) {
//        m_items.append(Item(query.value(0).toInt(), query.value(2).toString(), query.value(3).toString()));
//    }
//    endResetModel();
}

void ListModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void ListModel::searchIcd(const QString &icdText)
{
    emit searchIcd11(icdText);
}

void ListModel::listResult(QList<Item> items)
{
    beginResetModel();
    m_items = items;

    endResetModel();

    emit listReady();
}

void ListModel::searchIcd11Result(QList<Item> items)
{
    beginResetModel();
    m_items = items;

    endResetModel();

}


void ListModel::loadDrgs()
{
    emit startLoadDrgs();
}

