#include "icd11.h"

ICD11::ICD11()
{

}

ICD11::ICD11(unsigned int id, int conceptType, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(Type::ICD11, id, code, title, parentItem),
    coneptType(conceptType)
{

}

ICD11::ICD11(DRGBrowserModel *model):
    model(model)
{

}

void ICD11::setAttributesFromModel(const QModelIndex index)
{
    code = model->getItem(index)->getCode();
}

QString ICD11::getParentCode() const
{

}
