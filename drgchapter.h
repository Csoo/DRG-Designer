#ifndef DRGCHAPTER_H
#define DRGCHAPTER_H

#include "enum.hpp"
#include "treeitem.h"

class DRGChapter : public TreeItem {
public:
    DRGChapter();
    DRGChapter(unsigned int id, const QString &code = "", const QString &title = "", TreeItem *parentItem = nullptr);

};

#endif // DRGCHAPTER_H
