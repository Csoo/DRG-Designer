#ifndef DRGBROWSERMODEL_H
#define DRGBROWSERMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "treeitem.h"

class DRGChapter;

class DrgBrowserModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum TreeModelRoles
    {
        CodeRole = Qt::UserRole + 1,
        TitleRole
    };

    explicit DrgBrowserModel(const QString &data, QObject *parent = 0);
    ~DrgBrowserModel();

    /* QAbstractItemModel interface */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const override;

private:
    void setupModelData(QVariant data);
    TreeItem *rootItem;
    QHash<int, QByteArray> m_roleNameMapping;
};

#endif // DRGBROWSERMODEL_H
