#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QString>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(const QString &code = "", const QString &title = "", TreeItem *parentItem = 0);
    virtual ~TreeItem();

    void appendChild(TreeItem *child);
    bool removeChildren(int from, int count);

    TreeItem *child(int row);
    int childCount() const;
    virtual int columnCount() const;
    int row() const;
    TreeItem *parentItem();

    virtual QVariant data(int index) const;
    QString getTitle() const;
    void setTitle(const QString &value);
    QString getCode() const;
    void setCode(const QString &value);

    void setParentItem(TreeItem *parentItem);

protected:
    QString code;
    QString title;

private:
    QList<TreeItem*> m_childItems;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
