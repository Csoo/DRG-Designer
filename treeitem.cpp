#include "treeitem.h"
#include <QStringList>

TreeItem::TreeItem()
{

}

TreeItem::TreeItem(unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    id(id),
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
}

void TreeItem::setParentItem(TreeItem *parentItem)
{
    m_parentItem = parentItem;
}

QList<TreeItem *> TreeItem::getChildItems() const
{
    return m_childItems;
}

unsigned int TreeItem::getId() const
{
    return id;
}

bool TreeItem::insertChildren(int row, int count)
{
    if (row < 0 || row > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        TreeItem *item = new TreeItem(0);
        item->setParentItem(this);
        m_childItems.insert(row, item);
    }

    return true;
}

void TreeItem::setId(unsigned int value)
{
    id = value;
}
