#include "drgchapter.h"

DRGChapter::DRGChapter()
{

}

DRGChapter::DRGChapter(unsigned int id, const QString &code, const QString &title, TreeItem *parentItem):
    TreeItem(Type::DRG_CAPTER, id, code, title, parentItem)
{

}
