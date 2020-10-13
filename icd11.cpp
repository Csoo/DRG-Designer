#include "icd11.h"

ICD11::ICD11(unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(id, code, title, parentItem)
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
