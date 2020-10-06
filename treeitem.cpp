#include "treeitem.h"
#include <QStringList>

TreeItem::TreeItem(const QString &code, const QString &title, TreeItem *parent):
    code(code),
    title(title),
    m_parentItem(parent)
{

}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
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
    if (index < 0 || index > 1)
        return -1;

    switch (index)
    {
        case 0: return code;
        case 1: return title;
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
