#ifndef DRGCHAPTER_H
#define DRGCHAPTER_H

#include "treeitem.h"

class DRGChapter : public TreeItem
{
public:
    DRGChapter(const QString &code = "", const QString &title = "", TreeItem *parentItem = 0);

};

#endif // DRGCHAPTER_H
