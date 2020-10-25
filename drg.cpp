#include "drg.h"

QString DRG::getSimpleTitle() const
{
    return simpleTitle;
}


float DRG::getWeight() const
{
    return weight;
}


float DRG::getNormativeDay() const
{
    return normativeDay;
}

unsigned int DRG::getMaxDay() const
{
    return maxDay;
}


unsigned int DRG::getMinDay() const
{
    return minDay;
}

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
    DRG *d = dynamic_cast<DRG*>(model->getItem(index));
    title = d->getTitle();
    code = d->getCode();
    setAttributes(d->getMinDay(), d->getMaxDay(), d->getNormativeDay(), d->getWeight(), d->getSimpleTitle());
    propertiesChanged();
    attributeChanged();
}
