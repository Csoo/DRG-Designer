#include "drgbrowsermodel.h"

DrgBrowserModel::DrgBrowserModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_roleNameMapping[CodeRole] = "code";
    m_roleNameMapping[TitleRole] = "title";

    rootItem = new TreeItem("Code","Title");
    setupModelData(data);
}

DrgBrowserModel::~DrgBrowserModel()
{
    delete rootItem;
}

int DrgBrowserModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant DrgBrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != CodeRole && role != TitleRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

Qt::ItemFlags DrgBrowserModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant DrgBrowserModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex DrgBrowserModel::index(int row, int column, const QModelIndex &parent)
            const
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

QModelIndex DrgBrowserModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DrgBrowserModel::rowCount(const QModelIndex &parent) const
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

QHash<int, QByteArray> DrgBrowserModel::roleNames() const
{
    return m_roleNameMapping;
}

void DrgBrowserModel::setupModelData(QVariant data)
{
    TreeItem newItem("ADS", "Kutya");
    newItem.appendChild(new TreeItem("2dsa","Puli"));
    rootItem->appendChild(new TreeItem("ADS", "Kutya"));
}
