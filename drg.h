#ifndef DRG_H
#define DRG_H

#include "treeitem.h"

class DRG : public TreeItem
{
private:
    unsigned int minDay;
    unsigned int maxDay;
    float normativeDay;
    float weight;
    QString simpleTitle;
public:
    DRG(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
    void setAttributes(unsigned int minDay, unsigned int maxDay, float normativeDay, float weight, QString simpleTitle);
};

#endif // DRG_H
