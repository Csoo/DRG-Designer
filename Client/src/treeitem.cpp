#include "treeitem.h"
#include "treemodel.h"
#include <QStringList>

TreeItem::TreeItem()
{

}

TreeItem::TreeItem(int type, unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    id(id),
    type(type),
    code(code),
    title(title),
    m_parentItem(parentItem)
{

}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    item->setParentItem(this);
    m_childItems.append(item);
}

bool TreeItem::removeChildren(int from, int count)
{
    if (from < 0 || from + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(from);

    return true;
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size()) {
        qWarning("No child #%d for %s", row, title.toLatin1().data());
        return new TreeItem();
    }

    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return 2;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

QString TreeItem::getTitle() const
{
    return title;
}

void TreeItem::setTitle(const QString &value)
{
    title = value;
    propertiesChanged();
}

QVariant TreeItem::data(int index) const
{
    switch (index)
    {
        case 0: return id;
        case 1: return code;
        case 2: return title;
    }

    return -1;
}

QString TreeItem::getCode() const
{
    return code;
}

void TreeItem::setCode(const QString &value)
{
    code = value;
    emit propertiesChanged();
}

void TreeItem::setParentItem(TreeItem *parentItem)
{
    m_parentItem = parentItem;
}

QList<TreeItem *> TreeItem::getChildItems() const
{
    return m_childItems;
}

int TreeItem::findChildrenById(unsigned int id) const
{
    int i = 0;
    while (i < m_childItems.size() && m_childItems[i]->getId() != id) {
        i++;
    }

    return i < m_childItems.size() ? i : -1;
}

unsigned int TreeItem::getId() const
{
    return id;
}

void TreeItem::fetchFromModel(const QModelIndex &index)
{
    TreeItem *item = model->getItem(index);
    id = item->getId();
    type = item->getType();
    code = item->getCode();
    title = item->getTitle();
    m_index = index;
}

void TreeItem::updateModel()
{
    TreeItem *item = model->getItem(m_index);
    item->setId(id);
    item->setType(type);
    item->setCode(code);
    item->setTitle(title);
    emit propertiesChanged();
}

bool TreeItem::insertChildren(int row, int count)
{
    if (row < 0 || row > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        switch (this->getType()) {
            case Type::DRG_CAPTER : {
                TreeItem *item = new TreeItem(0,0,"CODE");
                item->setType(Type::DRG);
                item->setParentItem(this);
                m_childItems.insert(row, item);
                break;
            }
            case Type::DRG : {
                TreeItem *item = new TreeItem(0,0,"CODE");
                item->setType(Type::DRG_TYPE);
                //item->setType(Type::ICD11);
                item->setParentItem(this);
                m_childItems.insert(row, item);
                break;
            }
            case Type::DRG_TYPE : {
                TreeItem *item = new TreeItem(0,0,"CODE");
                item->setType(Type::ICD11);
                item->setParentItem(this);
                m_childItems.insert(row, item);
                break;
            }
            default: {
                ICD11 *item = new ICD11(0, 0, "CODE", "TITLE", 0, this);
                item->setType(Type::ICD11);
                item->setParentItem(this);
                m_childItems.insert(row, item);
                break;
            }
        }
    }

    return true;
}

void TreeItem::setId(unsigned int value)
{
    id = value;
    emit propertiesChanged();
}

int TreeItem::getType() const
{
    return type;
}

void TreeItem::setType(int value)
{
    type = value;
    propertiesChanged();
}

void TreeItem::setModel(TreeModel *value)
{
    model = value;
}

QString TreeItem::getDescription() const
{
    return description;
}

void TreeItem::setDescription(const QString &value)
{
    description = value;
    emit propertiesChanged();
}
