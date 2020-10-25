#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QString>
#include <QVariant>
#include "enum.hpp"

class TreeItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle NOTIFY propertiesChanged)
    Q_PROPERTY(QString code READ getCode NOTIFY propertiesChanged)
    Q_PROPERTY(unsigned int id READ getId NOTIFY propertiesChanged)
    Q_PROPERTY(int type READ getType NOTIFY propertiesChanged)
public:
    TreeItem();
    explicit TreeItem(int type, unsigned int id = 0, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
    virtual ~TreeItem();

    void appendChild(TreeItem *child);
    bool removeChildren(int from, int count);

    TreeItem *child(int row);
    int childCount() const;
    virtual int columnCount() const;
    int row() const;
    TreeItem *parentItem();

    virtual QVariant data(int index) const;
    Q_INVOKABLE QString getTitle() const;
    void setTitle(const QString &value);
    Q_INVOKABLE QString getCode() const;
    void setCode(const QString &value);

    void setParentItem(TreeItem *parentItem);

    QList<TreeItem *> getChildItems() const;
    int findChildrenById(unsigned int id) const;

    Q_INVOKABLE unsigned int getId() const;

    bool insertChildren(int row, int count);
    void setId(unsigned int value);

    int getType() const;
    void setType(int value);
signals:
    void propertiesChanged();

protected:
    int type;
    unsigned int id;
    QString code;
    QString title;
    QList<TreeItem*> m_childItems;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
