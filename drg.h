#ifndef DRG_H
#define DRG_H

#include "treeitem.h"
#include "drgbrowsermodel.h"
#include "QModelIndex"
#include "enum.hpp"

class DRGBrowserModel;

class DRG : public TreeItem
{
    Q_OBJECT
private:
    unsigned int minDay;
    unsigned int maxDay;
    float normativeDay;
    float weight;
    QString simpleTitle;
    DRGBrowserModel *model;
public:
    DRG();
    DRG(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
    DRG(DRGBrowserModel *model);
    void setAttributes(unsigned int minDay, unsigned int maxDay, float normativeDay, float weight, QString simpleTitle);

    Q_INVOKABLE void setAttributesFromModel(const QModelIndex &index);
};

#endif // DRG_H
