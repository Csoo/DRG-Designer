#include "selection.h"

ICD11 *Selection::icd() const
{
    return m_icd;
}

Selection::Selection(TreeModel *drgBrowser):
    m_icd(new ICD11(0,0,"asd","title")),
    drgBrowser(drgBrowser)
{

}

void Selection::select(const QModelIndex &index)
{
    ICD11 *icd = dynamic_cast<ICD11*>(drgBrowser->getItem(index));
    m_icd = icd;
    qDebug() << m_icd->getCode();
}
