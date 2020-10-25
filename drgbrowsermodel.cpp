#include "drgbrowsermodel.h"
#include <QtDebug>

#define MAX_NODES 100

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
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    bool ok = parentItem->insertChildren(row, count);
    endInsertRows();

    return ok;
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

bool DRGBrowserModel::isParent(const QModelIndex &parent, const QModelIndex &child) const
{
    TreeItem *parentItem = this->getItem(parent);
    TreeItem *childItem = this->getItem(child);

    return parentItem->getChildItems().indexOf(childItem) != -1;
}

void DRGBrowserModel::loadIcd(unsigned int id, const QModelIndex &parent)
{
    qDebug() << id << " icd11 loading..";
    TreeItem *parentItem = this->getItem(parent);
    QSqlQuery query = db->listIcdDrgRelation(id);
    removeRows(0, parentItem->childCount(), parent);

    qDebug() << id << " icd11 loading..";

    while (query.next()) {
        //ICD11 *icd = new ICD11(query.value(0).toInt(), query.value(2).toString(), query.value(3).toString());
        insertRow(0, parent);
        parentItem->child(0)->setId(query.value(0).toInt());
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

int DRGBrowserModel::getType(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getType();
}

QString DRGBrowserModel::getTitle(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getTitle();
}

QString DRGBrowserModel::getCode(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getCode();
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
        DRGChapter *chapter = new DRGChapter(query.value(0).toInt(), query.value(2).toString(),
                                             query.value(3).toString());
        rootItem->appendChild(chapter);
    }

//    for (auto chapter : rootItem->getChildItems()) {
//        QSqlQuery entityQuery = db->listDrgEntities(chapter->getId());
//        while (entityQuery.next()) {
//            DRG *entity = new DRG(entityQuery.value(0).toInt(), entityQuery.value(3).toString(),
//                                  entityQuery.value(4).toString());
//            entity->setAttributes(entityQuery.value(6).toInt(), entityQuery.value(7).toInt(), entityQuery.value(8).toInt(),entityQuery.value(9).toInt(), entityQuery.value(5).toString());
//            //entity->appendChild(new ICD11);
//            chapter->appendChild(entity);
//        }

////        for (auto drg : chapter->getChildItems()) {
////            QSqlQuery typeQuery = db->listDrgTypes(drg->getId());
////            while (typeQuery.next()) {
////                TreeItem *type = new TreeItem(Type::DRG_TYPE);
////                type->setTitle(typeQuery.value(1).toString());
////                type->setId(typeQuery.value(0).toInt());
////                drg->appendChild(type);
////            }
////        }
//    }

    for (auto chapter : rootItem->getChildItems()) {
        QSqlQuery drgQuery = db->listOrderedDrgBno(chapter->getId());
        drgQuery.next();
        int currentDrg = 0, currentType = 0;
        int typeIdx = 0, drgIdx = -1;
        while (drgQuery.next()) {
            DRG *drg = new DRG(drgQuery.value(0).toInt(), drgQuery.value(1).toString(), drgQuery.value(2).toString());
            TreeItem *type = new TreeItem(Type::DRG_TYPE, drgQuery.value(3).toInt(), drgQuery.value(4).toString());
            ICD11 *icd = new ICD11(drgQuery.value(5).toInt(), 0, drgQuery.value(6).toString(), drgQuery.value(7).toString());
            if (currentDrg != drgQuery.value(0).toInt()) {
                drgIdx++;
                typeIdx = 0;
                currentDrg = drgQuery.value(0).toInt();
                currentType = drgQuery.value(3).toInt();
                type->appendChild(icd);
                drg->appendChild(type);
                chapter->appendChild(drg);
            } else {
                if (currentType != drgQuery.value(3).toInt()) {
                    typeIdx++;
                    currentType = drgQuery.value(3).toInt();
                    chapter->child(drgIdx)->appendChild(type);
                } else {
                    chapter->child(drgIdx)->child(typeIdx)->appendChild(icd);
                }
                drgId++;
            }
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

QVector<QModelIndex> DRGBrowserModel::getItemIndexes(const QModelIndex &index) const
{
    QVector<QModelIndex> nodes;

    if (getItem(index)->childCount() == 0) {
        nodes.append(index);
        return nodes;
    }

    TreeItem *item = this->getItem(index);
    QVector<TreeItem*> nodeList { item };

    while (!nodeList.isEmpty()) {
        item = nodeList.first();
        for (auto child : item->getChildItems()) {
            nodeList.append(child);
        }
        nodes.append(createIndex(nodeList.first()->row(), 0, nodeList.first()));
        nodeList.pop_front();
    }

    return nodes;
}

void DRGBrowserModel::setDrgAttributes(const QModelIndex &chapterIndex)
{
    TreeItem *chapter = this->getItem(chapterIndex);
    QSqlQuery query = db->listDrgEntities(chapter->getId());
    while (query.next()) {
        int childIdx = chapter->findChildrenById(query.value(0).toInt());
        if (childIdx != -1) {
            qDebug() << "setting drg attributes for: " << query.value(0).toInt();
            TreeItem  *base = chapter->getChildItems()[childIdx];
            DRG *drg = dynamic_cast<DRG*>(base);
            if (drg) {
                drg->setAttributes(query.value(6).toInt(),
                                   query.value(7).toInt(),
                                   query.value(8).toInt(),
                                   query.value(9).toFloat(),
                                   query.value(5).toString());
            }
        }
    }
}

void DRGBrowserModel::loadPostCoord(unsigned int id, int type)
{
    int currentAxisId = -1;
    int axisIdx = 0;
    QSqlQuery icdQuery = db->listIcdLinea(id, type)[0];
    QSqlQuery postCoordQuery = db->listIcdLinea(id, type)[1];
    qDebug() << "loading post-coordinations..";
    beginResetModel();
    while (postCoordQuery.next()) {
        TreeItem *axis = new TreeItem(Type::AXIS, postCoordQuery.value(1).toInt(), "AXIS", postCoordQuery.value(2).toString());
        ICD11 *icd = new ICD11(postCoordQuery.value(5).toInt(),
                               postCoordQuery.value(6).toInt(),
                               postCoordQuery.value(7).isNull() ? postCoordQuery.value(8).toString() : postCoordQuery.value(7).toString(),
                               postCoordQuery.value(10).toString());
        if (currentAxisId != postCoordQuery.value(1).toInt()) {
            currentAxisId = postCoordQuery.value(1).toInt();
            axisIdx = 0;
            axis->appendChild(icd);
            rootItem->appendChild(axis);
        } else {
            rootItem->child(axisIdx)->appendChild(icd);
            axisIdx++;
        }
    }
}
