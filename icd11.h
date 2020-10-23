#ifndef ICD11_H
#define ICD11_H

#include "treeitem.h"
#include "drgbrowsermodel.h"
#include "QModelIndex"
#include "enum.hpp"

class DRGBrowserModel;

class ICD11 : public TreeItem
{
    Q_OBJECT
private:
    DRGBrowserModel *model;
public:
    ICD11();
    ICD11(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
    ICD11(DRGBrowserModel *model);

    Q_INVOKABLE void setAttributesFromModel(const QModelIndex index);
    Q_INVOKABLE QString getParentCode() const;
};

#endif // ICD11_H
