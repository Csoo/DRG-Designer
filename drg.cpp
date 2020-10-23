#include "drg.h"

DRG::DRG()
{

}

DRG::DRG(unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(Type::DRG, id, code, title, parentItem)
{

}

DRG::DRG(DRGBrowserModel *model):
    model(model)
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

void DRG::setAttributesFromModel(const QModelIndex &index)
{
    title = model->getItem(index)->getTitle();
}
