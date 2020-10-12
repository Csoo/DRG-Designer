#include "drgbrowsermodel.h"
#include <QtDebug>

DRGBrowserModel::DRGBrowserModel(QObject *parent)
    : QAbstractItemModel(parent),
      dbError(""),
      rootItem(new TreeItem(0))
{

}

TreeItem *DRGBrowserModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant DRGBrowserModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool DRGBrowserModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        rootItem->setCode("TEST");
        rootItem->setTitle("TITLE");
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex DRGBrowserModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex DRGBrowserModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DRGBrowserModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int DRGBrowserModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool DRGBrowserModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);

    return parentItem->childCount() > 0;
}

bool DRGBrowserModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void DRGBrowserModel::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}

QVariant DRGBrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != CodeRole && role != TitleRole && role != IdRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

bool DRGBrowserModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DRGBrowserModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool DRGBrowserModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "inset rows" << row << count;
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    bool ok = parentItem->insertChildren(row, count);
    endInsertRows();

    return ok;
}

bool DRGBrowserModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool DRGBrowserModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool ok = false;
    TreeItem *parentItem = this->getItem(parent);
    if (!parentItem)
        return ok;

    beginRemoveRows(parent, row, row + count - 1);
    ok = parentItem->removeChildren(row, count);
    endRemoveRows();
    return ok;
}

bool DRGBrowserModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}

QHash<int, QByteArray> DRGBrowserModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[CodeRole] = "code";
    names[TitleRole] = "title";
    return names;
}

bool DRGBrowserModel::connectToDatabase()
{
    if (!db->connectDatabase(&dbError)) {
        return false;
    }
    return true;
}

void DRGBrowserModel::loadIcd(unsigned int id, const QModelIndex &parent)
{
    //not loaded
    qDebug() << id << " icd11 loading..";
    TreeItem *parentItem = this->getItem(parent);
    QSqlQuery query = db->listIcdDrgRelation(id);
    removeRows(0, parentItem->childCount(), parent);

    qDebug() << id << " icd11 loading..";

    while (query.next()) {
        //ICD11 *icd = new ICD11(query.value(0).toInt(), query.value(2).toString(), query.value(3).toString());
        insertRow(0, parent);
        parentItem->child(0)->setCode(query.value(2).toString());
        parentItem->child(0)->setTitle(query.value(3).toString());
    }
    qDebug() << id << " icd11 loaded!";
}

void DRGBrowserModel::setDb(Database *value)
{
    db = value;
}

unsigned int DRGBrowserModel::depth(const QModelIndex &parent) const
{
    unsigned int depth = 0;
    TreeItem *parentTreeItem = this->getItem(parent);

    while (parentTreeItem->parentItem() != nullptr) {
        parentTreeItem = parentTreeItem->parentItem();
        depth++;
    }

    return depth;
}

unsigned int DRGBrowserModel::getId(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);

    return parentItem->getId();
}

bool DRGBrowserModel::isEmpty(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);

    return (parentItem->childCount() == 1 && parentItem->child(0)->getCode() == 0)
            || parentItem->childCount() == 0;
}

QString DRGBrowserModel::getDbError() const
{
    return dbError;
}

void DRGBrowserModel::loadDrgEntities(int drgId)
{
    beginResetModel();
    qDebug() << "load items";
    QSqlQuery query = db->listDrgChapters(drgId);
    QList<unsigned int> chapterIds;
    while (query.next()) {
        unsigned int chapterId = query.value(0).toInt();
        DRGChapter *chapter = new DRGChapter(chapterId, query.value(2).toString(),
                                             query.value(3).toString());
        chapterIds.append(chapterId);
        rootItem->appendChild(chapter);
    }

    for (auto chapter : rootItem->getChildItems()) {
        QSqlQuery entityQuery = db->listDrgEntities(chapter->getId());
        while (entityQuery.next()) {
            DRG *entity = new DRG(entityQuery.value(0).toInt(), entityQuery.value(3).toString(),
                                  entityQuery.value(4).toString());
            entity->setAttributes(entityQuery.value(6).toInt(), entityQuery.value(7).toInt(), entityQuery.value(8).toInt(),entityQuery.value(9).toInt(), entityQuery.value(5).toString());
            entity->appendChild(new ICD11(0));
            chapter->appendChild(entity);
        }
    }
    endResetModel();

//    TreeItem *Dog = new TreeItem("01","Kutya");
//    TreeItem *newItem = new TreeItem("111", "Puli");
//    Dog->appendChild(newItem);
//    newItem->appendChild(new TreeItem("112", "Gömbi"));
//    newItem->appendChild(new TreeItem("112", "Picur"));
//    rootItem->appendChild(Dog);
//    TreeItem *Cat = new TreeItem("02", "Macska");
//    Cat->appendChild(new TreeItem("111", "Sziami"));
//    rootItem->appendChild(Cat);
}
