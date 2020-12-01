#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QModelIndex>
#include "enum.hpp"

class TreeModel;

class TreeItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY propertiesChanged)
    Q_PROPERTY(QString code READ getCode WRITE setCode NOTIFY propertiesChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY propertiesChanged)
    Q_PROPERTY(unsigned int id READ getId WRITE setId NOTIFY propertiesChanged)
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
    Q_INVOKABLE virtual void fetchFromModel(const QModelIndex &index);
    Q_INVOKABLE virtual void updateModel();

    bool insertChildren(int row, int count);
    void setId(unsigned int value);

    int getType() const;
    void setType(int value);
    void setModel(TreeModel *value);

    QString getDescription() const;
    void setDescription(const QString &value);

signals:
    void propertiesChanged();

protected:
    unsigned int id;
    int type;
    QString code;
    QString title;
    QString description;

    QList<TreeItem*> m_childItems;
    TreeItem *m_parentItem;
    TreeModel *model;

    QModelIndex m_index;
};

#endif // TREEITEM_H
