#ifndef SELECTION_H
#define SELECTION_H

#include <QDebug>
#include <QObject>
#include <QModelIndex>
#include "treeitem.h"
#include "icd11.h"
#include "treemodel.h"

class Selection : public QObject
{
    Q_OBJECT
    ICD11 *m_icd;
    TreeModel *drgBrowser;

public:
    Selection(TreeModel *drgBrowser);

    Q_INVOKABLE void select(const QModelIndex &index);
    ICD11 *icd() const;
};

#endif // SELECTION_H
