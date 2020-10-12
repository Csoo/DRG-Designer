#include "drg.h"

DRG::DRG(unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(id, code, title, parentItem)
{

}

void DRG::setAttributes(unsigned int minDay, unsigned int maxDay, float normativeDay, float weight, QString simpleTitle)
{
    this->minDay = minDay;
    this->maxDay = maxDay;
    this->normativeDay = normativeDay;
    this->weight = weight;
    this->simpleTitle = simpleTitle;
}
