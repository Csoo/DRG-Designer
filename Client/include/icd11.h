#ifndef ICD11_H
#define ICD11_H

#include "treeitem.h"
#include "QModelIndex"
#include "enum.hpp"
#include "repository.h"
#include <QThread>

class TreeModel;

class ICD11 : public TreeItem
{
    Q_OBJECT
    Q_PROPERTY(int conceptType READ getConceptType WRITE setConceptType NOTIFY attributesChanged)
    Q_PROPERTY(int isApproved READ getIsApproved WRITE setIsApproved NOTIFY isApprovedChanged)
    Q_PROPERTY(int postCoordCount READ getPostCoordCount NOTIFY postCoordCountChanged)
private:
    bool isApproved;
    int conceptType;
    int icd10Id;
    QString icd10Title;
    QString icd10Code;
    QList<ICD11*> postCoordinations;

    Repository *db;
    QThread *dbThread;
public:
    ICD11();
    ICD11(Repository *db, QThread *dbThread);
    ICD11(unsigned int id, int conceptType, const QString &code = "", const QString &title = "", bool approved = false, TreeItem *parentItem = nullptr);
    ~ICD11();

    Q_INVOKABLE virtual void fetchFromModel(const QModelIndex &index);
    Q_INVOKABLE QString getParentCode() const;
    Q_INVOKABLE void loadRecommandation();
    Q_INVOKABLE void loadDetails();
    Q_INVOKABLE int getPostCoordCount() const;
    Q_INVOKABLE bool hasPostCoord() const;
    Q_INVOKABLE QString getFullCode() const;
    Q_INVOKABLE QString getPostCode(int idx) const;
    Q_INVOKABLE QString getPostTitle(int idx) const;
    Q_INVOKABLE void approveICD(const QModelIndex &item);
    Q_INVOKABLE void updatePostCoord(unsigned int id, const QString &code, const QString &title);
    Q_INVOKABLE void clearPostCoord();
    int getConceptType() const;
    bool getIsApproved() const;

    void setIsApproved(bool value);
    void setConceptType(int value);

    void addPostCoordination(ICD11 *icd);

    QList<ICD11 *> getPostCoordinations() const;

    void setPostCoordinations(const QList<ICD11 *> &value);

    QString getIcd10Code() const;
    void setIcd10Code(const QString &value);

    QString getIcd10Title() const;
    void setIcd10Title(const QString &value);

    int getIcd10Id() const;
    void setIcd10Id(int value);

public slots:
    void recommendationResult(bool noRecommendation, ICD11 *rec);
    void detailsResult(ICD11 *icd);
signals:
    void attributesChanged();
    void isApprovedChanged();
    void icdChanged();
    void startLoadRecommendation(int icd10Id);
    void whoSuggestionReady(bool noRecommedation);
    void startLoadICDDetails(int icdId, int conceptType);
    void postCoordCountChanged();
};

#endif // ICD11_H
