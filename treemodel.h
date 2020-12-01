#ifndef TreeModel_H
#define TreeModel_H

#include <QAbstractItemModel>
#include "treeitem.h"
#include "drgchapter.h"
#include "drg.h"
#include "icd11.h"
#include "enum.hpp"
#include "repository.h"
#include <QThread>

#include <QSqlQuery>
#include "../icd-project/backendDatabase/database.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(QString dbError READ getDbError())

public:
    enum TreeModelRoles
    {
        IdRole = Qt::UserRole + 1,
        CodeRole,
        TitleRole
    };
    explicit TreeModel(Repository *db, QThread *dbThread, QObject *parent = nullptr);
    ~TreeModel();

    TreeItem *getItem(const QModelIndex &index) const;
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

    QString getDbError() const;

    //void setDb(Database *value);

    Q_INVOKABLE unsigned int depth(const QModelIndex &parent) const;
    Q_INVOKABLE unsigned int getId(const QModelIndex &parent) const;
    Q_INVOKABLE int getType(const QModelIndex &parent) const;
    Q_INVOKABLE QString getTitle(const QModelIndex &parent) const;
    Q_INVOKABLE QString getCode(const QModelIndex &parent) const;
    Q_INVOKABLE bool isEmpty(const QModelIndex &parent) const;
    Q_INVOKABLE bool isParent(const QModelIndex &parent, const QModelIndex &child) const;
    Q_INVOKABLE void loadIcd(unsigned int id, const QModelIndex &parent);
    Q_INVOKABLE void loadDrgEntities(int drgId);
    Q_INVOKABLE QVector<QModelIndex> getItemIndexes(const QModelIndex &index) const;
    Q_INVOKABLE void setDrgAttributes(const QModelIndex &chapterIndex);
    Q_INVOKABLE void loadPostCoord(unsigned int id, int type);
    Q_INVOKABLE void loadChildren(const QModelIndex &parent);
    Q_INVOKABLE bool isSelected(const QModelIndex &item);
    Q_INVOKABLE void setSelectedIndexes(const QModelIndexList &value);
    Q_INVOKABLE bool isApproved(const QModelIndex &item);
    Q_INVOKABLE QString getTitleOfBaseItem(int idx);
    Q_INVOKABLE QModelIndex getIndexOfBaseItem(int idx);

private:

    //Database *db;
    Repository *db1;
    QThread *dbThread;
    QString dbError;

    TreeItem *rootItem;
    QModelIndexList selectedIndexes;
    QHash<int, QByteArray> m_roleNameMapping;

public slots:
    void drgBrowserResult(TreeItem *root);
    void drgChapterResult(int chapterId, const QList<DRG*> &drgList);
    void icdEntityResult(const QModelIndex &icdIndex , const ICD11 &icd);
    void postCoordResult(int numberOfAxis, ICD11 *root);
    void loadChildrenResult(const QList<ICD11*> &icdList, const QModelIndex &parent);

signals:
    void loadDrgBrowser(int drgId);
    void loadChapterDrg(int chapterId);
    void loadIcdEntity(int icdId, int conceptType);
    void loadPostCoordTree(int icdId, int conceptType);
    void loadItemChildren(unsigned int id, int type, const QModelIndex &parent);
    void postCoordReady(int numberOfAxis);
};

#endif
