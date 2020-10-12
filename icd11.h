#ifndef ICD11_H
#define ICD11_H

#include "treeitem.h"

class ICD11 : public TreeItem
{
public:
    ICD11(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);
};

#endif // ICD11_H
