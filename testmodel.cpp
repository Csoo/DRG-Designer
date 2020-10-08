#include "testmodel.h"
#include <QtDebug>

TestModel::TestModel(QObject *parent)
    : QAbstractItemModel(parent)
{

    rootItem = new TreeItem;

    rootItem->setCode("TEST2");
    rootItem->setTitle("TITLE2");

    setUpModel();
}

TreeItem *TestModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TestModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool TestModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        rootItem->setCode("TEST");
        rootItem->setTitle("TITLE");
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex TestModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex TestModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TestModel::rowCount(const QModelIndex &parent) const
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

int TestModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool TestModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->childCount() > 0;
}

bool TestModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void TestModel::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != CodeRole && role != TitleRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

bool TestModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TestModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool TestModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
}

bool TestModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool TestModel::removeRows(int row, int count, const QModelIndex &parent)
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

bool TestModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}

QHash<int, QByteArray> TestModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[CodeRole] = "code";
    names[TitleRole] = "title";
    return names;
}

void TestModel::setUpModel()
{
    TreeItem *Dog = new TreeItem("01","Kutya");
    TreeItem *newItem = new TreeItem("111", "Puli");
    Dog->appendChild(newItem);
    newItem->appendChild(new TreeItem("112", "Gömbi"));
    newItem->appendChild(new TreeItem("112", "Picur"));
    rootItem->appendChild(Dog);
    TreeItem *Cat = new TreeItem("02", "Macska");
    Cat->appendChild(new TreeItem("111", "Sziami"));
    rootItem->appendChild(Cat);
}
