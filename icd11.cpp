#include "icd11.h"

int ICD11::getConceptType() const
{
    return conceptType;
}

bool ICD11::getIsApproved() const
{
    return isApproved;
}

void ICD11::setIsApproved(bool value)
{
    isApproved = value;
    isApprovedChanged();
}

ICD11::ICD11()
{
    
}

ICD11::ICD11(unsigned int id, int conceptType, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(Type::ICD11, id, code, title, parentItem),
    conceptType(conceptType)
{
    isApproved = false;
}

ICD11::ICD11(DRGBrowserModel *model):
    model(model)
{

}

void ICD11::setAttributesFromModel(const QModelIndex index)
{
    ICD11 *icd = dynamic_cast<ICD11*>(model->getItem(index));
    title = icd->getTitle();
    code = icd->getCode();
    id = 933570887;
    attributesChanged();
}

QString ICD11::getParentCode() const
{

}
