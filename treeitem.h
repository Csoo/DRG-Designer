#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QString>
#include <QVariant>

class TreeItem
{
public:
    explicit TreeItem(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
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

    QList<TreeItem *> getChildItems() const;

    unsigned int getId() const;

    bool insertChildren(int row, int count);
protected:
    unsigned int id;
    QString code;
    QString title;
    QList<TreeItem*> m_childItems;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
