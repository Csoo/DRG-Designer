 #include "treemodel.h"
#include <QtDebug>

#define MAX_NODES 100

TreeModel::TreeModel(Repository *db, QThread *dbThread, QObject *parent)
    : QAbstractItemModel(parent),
      db1(db),
      dbThread(dbThread),
      dbError(""),
      rootItem(new TreeItem())
{

}

TreeModel::~TreeModel()
{
    qDebug() << "~TreeModel";
    delete rootItem;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid()) {
        parentItem = rootItem;
    }
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);

    return parentItem->childCount() > 0;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != CodeRole && role != TitleRole && role != IdRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    bool ok = parentItem->insertChildren(row, count);
    endInsertRows();

    return ok;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
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

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[IdRole] = "id";
    names[CodeRole] = "code";
    names[TitleRole] = "title";
    return names;
}

bool TreeModel::isParent(const QModelIndex &parent, const QModelIndex &child) const
{
    TreeItem *parentItem = this->getItem(parent);
    TreeItem *childItem = this->getItem(child);

    return parentItem->getChildItems().indexOf(childItem) != -1;
}

void TreeModel::loadIcd(unsigned int id, const QModelIndex &parent)
{
    qDebug() << id << " icd11 loading..";
    //emit loadIcdEntity(,);
//    TreeItem *parentItem = this->getItem(parent);
//    QSqlQuery query = db->listIcdDrgRelation(id);
//    removeRows(0, parentItem->childCount(), parent);

//    qDebug() << id << " icd11 loading..";

//    while (query.next()) {
//        //ICD11 *icd = new ICD11(query.value(0).toInt(), query.value(2).toString(), query.value(3).toString());
//        insertRow(0, parent);
//        parentItem->child(0)->setId(query.value(0).toInt());
//        parentItem->child(0)->setCode(query.value(2).toString());
//        parentItem->child(0)->setTitle(query.value(3).toString());
//    }
//    qDebug() << id << " icd11 loaded!";
}

//void TreeModel::setDb(Database *value)
//{
//    db = value;
//}

unsigned int TreeModel::depth(const QModelIndex &parent) const
{
    unsigned int depth = 0;
    TreeItem *parentTreeItem = this->getItem(parent);

    while (parentTreeItem->parentItem() != nullptr) {
        parentTreeItem = parentTreeItem->parentItem();
        depth++;
    }

    return depth;
}

unsigned int TreeModel::getId(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getId();
}

int TreeModel::getType(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getType();
}

QString TreeModel::getTitle(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getTitle();
}

QString TreeModel::getCode(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);
    return parentItem->getCode();
}

bool TreeModel::isEmpty(const QModelIndex &parent) const
{
    TreeItem *parentItem = this->getItem(parent);

    return (parentItem->childCount() == 1 && parentItem->child(0)->getCode().isEmpty())
            || parentItem->childCount() == 0;
}

QString TreeModel::getDbError() const
{
    return dbError;
}

void TreeModel::loadDrgEntities(int drgId)
{
    loadDrgBrowser(drgId);

//    qDebug() << "load items";
//    QSqlQuery query = db->listDrgChapters(drgId);
//    QList<unsigned int> chapterIds;
//    while (query.next()) {
//        DRGChapter *chapter = new DRGChapter(query.value(0).toInt(), query.value(2).toString(),
//                                             query.value(3).toString());
//        rootItem->appendChild(chapter);
//    }

//    for (auto chapter : rootItem->getChildItems()) {
//        QSqlQuery drgQuery = db->listOrderedDrgBno(chapter->getId());
//        unsigned int currentDrg = -1, currentType = -1;
//        int typeIdx = 0, drgIdx = -1;
//        while (drgQuery.next()) {
//            DRG *drg = new DRG(drgQuery.value(0).toInt(), drgQuery.value(1).toString(), drgQuery.value(2).toString());
//            TreeItem *type = new TreeItem(Type::DRG_TYPE, drgQuery.value(3).toInt(), drgQuery.value(4).toString());
//            ICD11 *icd = new ICD11(drgQuery.value(5).toInt(), -1, drgQuery.value(6).toString(), drgQuery.value(7).toString(), drgQuery.value(8).toInt());
//            if (currentDrg != drg->getId()) {
//                typeIdx = 0;
//                drgIdx++;
//                currentDrg = drgQuery.value(0).toInt();
//                currentType = drgQuery.value(3).toInt();
//                chapter->appendChild(drg);
//                if (!drgQuery.value(3).isNull())
//                    chapter->child(drgIdx)->appendChild(type);
//            } else {
//                if (currentType != type->getId()) {
//                    typeIdx++;
//                    currentType = drgQuery.value(3).toInt();
//                    if (!drgQuery.value(3).isNull())
//                        chapter->child(drgIdx)->appendChild(type);
//                }
//            }
//            if (!drgQuery.value(5).isNull())
//                chapter->child(drgIdx)->child(typeIdx)->appendChild(icd);
//        }
//    }

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

QVector<QModelIndex> TreeModel::getItemIndexes(const QModelIndex &index) const
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

void TreeModel::setDrgAttributes(const QModelIndex &chapterIndex)
{
    TreeItem *chapter = this->getItem(chapterIndex);
    emit loadChapterDrg(chapter->getId());

//    QSqlQuery query = db->listDrgEntities(chapter->getId());
//    while (query.next()) {
//        int childIdx = chapter->findChildrenById(query.value(0).toInt());
//        if (childIdx != -1) {
//            DRG *drg = dynamic_cast<DRG*>(chapter->getChildItems()[childIdx]);
//            if (drg) {
//                drg->setAttributes(query.value(6).toInt(),
//                                   query.value(7).toInt(),
//                                   query.value(8).toInt(),
//                                   query.value(9).toFloat(),
//                                   query.value(5).toString());
//            }
//        }
//    }
}

void TreeModel::loadPostCoord(unsigned int id, int type)
{
    emit loadPostCoordTree(id, type);
//    int currentAxisId = -1;
//    int axisIdx = -1;
//    QList<QSqlQuery> queries = db->listIcd11Linea(id, type);
//    QSqlQuery icdQuery = queries[0];
//    QSqlQuery postCoordQuery = queries[1];
//    qDebug() << "id: " << id << type << "loading post-coordinations..";
//    beginResetModel();

//    rootItem->removeChildren(0, rootItem->childCount());

//    while (postCoordQuery.next()) {
//        TreeItem *axis = new TreeItem(Type::AXIS, postCoordQuery.value(1).toInt(), postCoordQuery.value(2).toString(), "");
//        ICD11 *icd = new ICD11(postCoordQuery.value(5).toInt(),
//                               postCoordQuery.value(6).toInt(),
//                               postCoordQuery.value(7).isNull() ? postCoordQuery.value(8).toString() : postCoordQuery.value(7).toString(),
//                               postCoordQuery.value(10).toString());
//        if (currentAxisId != postCoordQuery.value(1).toInt()) {
//            currentAxisId = postCoordQuery.value(1).toInt();
//            axisIdx++;
//            axis->appendChild(icd);
//            rootItem->appendChild(axis);
//        } else {
//            rootItem->child(axisIdx)->appendChild(icd);
//        }
//    }
//    endResetModel();
}

void TreeModel::loadChildren(const QModelIndex &parent)
{
    TreeItem *item = this->getItem(parent);
    emit loadItemChildren(item->getId(), 0, parent);
//    qDebug() << "postchild loading" << id << type;
//    TreeItem *parentItem = this->getItem(parent);
//    QSqlQuery query = db->getPostcoordinationChild(id, type);
//    if (parentItem->childCount() == 0) {
//        while (query.next()) {
//            ICD11 *icd = new ICD11(query.value(0).toInt(), query.value(1).toInt(),
//                      query.value(2).toString(),
//                      query.value(5).toString());
//            insertRows(parentItem->row(), 1, parent);
//            parentItem->appendChild(icd);
//        }
//    }
}

bool TreeModel::isSelected(const QModelIndex &itemIdx)
{
    return selectedIndexes.indexOf(itemIdx) != -1;
}

void TreeModel::setSelectedIndexes(const QModelIndexList &value)
{
    selectedIndexes = value;
}

bool TreeModel::isApproved(const QModelIndex &item)
{
    ICD11 *icd = dynamic_cast<ICD11*>(this->getItem(item));
    if (icd) {
        return icd->getIsApproved();
    }

    return false;
}

QString TreeModel::getTitleOfBaseItem(int idx)
{
    if (idx >= rootItem->childCount())
        return "Hiba";
    return rootItem->getChildItems()[idx]->getCode();
}

QModelIndex TreeModel::getIndexOfBaseItem(int idx)
{    if (idx >= rootItem->childCount())
        return QModelIndex();
    return createIndex(rootItem->getChildItems()[idx]->row(), 0, rootItem->getChildItems()[idx]);
}

//void TreeModel::approveICD(const QModelIndex &item, const ICD11 &icd)
//{
    //    ICD11 *icdCurrnet = dynamic_cast<ICD11*>(this->getItem(item));
    //    icdCurrnet->setIcd10Id(icdCurrnet->getId());
    //    icdCurrnet->setIcd10Code(icdCurrnet->getCode());
    //    icdCurrnet->setIcd10Title(icdCurrnet->getTitle());
    //    icdCurrnet->setId(icd.getId());
    //    icdCurrnet->setTitle(icd.getTitle());
    //    icdCurrnet->setConceptType(icd.getConceptType());
    //    icdCurrnet->setCode(icd.getCode());
    //    icdCurrnet->setPostCoordinations(icd.getPostCoordinations());
    //    icdCurrnet->setIsApproved(true);
//}

void TreeModel::drgBrowserResult(TreeItem *root)
{
    beginResetModel();
    rootItem = root;
    endResetModel();
}

void TreeModel::drgChapterResult(int chapterId, const QList<DRG *> &drgList)
{
    int chapterIdx = rootItem->findChildrenById(chapterId);
    if (chapterIdx == -1) {
        return;
    }
    TreeItem *chapter = rootItem->getChildItems()[chapterIdx];
    for (int i = 0; i < drgList.size(); i++) {
        int childIdx = chapter->findChildrenById(drgList[i]->getId());
        if (childIdx != -1) {
            DRG *drg = dynamic_cast<DRG*>(chapter->getChildItems()[childIdx]);
            if (drg) {
                drg->setAttributes(drgList[i]->getMinDay(),
                                   drgList[i]->getMaxDay(),
                                   drgList[i]->getNormativeDay(),
                                   drgList[i]->getWeight(),
                                   drgList[i]->getSimpleTitle());
            }
        }
    }
}

void TreeModel::icdEntityResult(const QModelIndex &icdIndex, const ICD11 &icd)
{
    ICD11 *icdItem = dynamic_cast<ICD11*>(this->getItem(icdIndex));
}

void TreeModel::postCoordResult(int numberOfAxis, ICD11 *root)
{
    beginResetModel();
    if (numberOfAxis > 0)
        rootItem = root;
    else
        rootItem->getChildItems().clear();
    endResetModel();
    emit postCoordReady(numberOfAxis);
}

void TreeModel::loadChildrenResult(const QList<ICD11 *> &icdList, const QModelIndex &parent)
{
    TreeItem *item = this->getItem(parent);
    //removeRows(item->row(), 1, parent);
    beginRemoveRows(parent, 0, 0);
    item->removeChildren(0, 1);
    endRemoveRows();
    beginInsertRows(parent, 0, icdList.size() - 1);
    qDebug() << "insert to" << 0 << icdList.size() - 1;
    for (auto icd : icdList) {
        item->appendChild(icd);
        //qDebug() << "Item inserted" << icd->getTitle() << "to" << item->getTitle();
    }
    endInsertRows();
}
