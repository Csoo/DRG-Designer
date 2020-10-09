#include "drgchapter.h"

DRGChapter::DRGChapter(const QString &code, const QString &title, TreeItem *parentItem)
{
    this->code = code;
    this->title = title;
    this->m_parentItem = parentItem;
}
