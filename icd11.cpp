#include "icd11.h"

int ICD11::getConceptType() const
{
    return conceptType;
}

ICD11::ICD11()
{
    
}

ICD11::ICD11(unsigned int id, int conceptType, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(Type::ICD11, id, code, title, parentItem),
    conceptType(conceptType)
{

}

ICD11::ICD11(DRGBrowserModel *model):
    model(model)
{

}

void ICD11::setAttributesFromModel(const QModelIndex index)
{
    code = model->getItem(index)->getCode();
    attributesChanged();
}

QString ICD11::getParentCode() const
{

}
