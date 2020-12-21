#ifndef BITMAP_H
#define BITMAP_H

#include <QtWidgets>

#include "windows.h"

// структура ячейки
struct TCell
{
    unsigned h; // высота ячейки
    unsigned w; // ширина ячейки
};

#pragma pack(push, 1)

struct TBitMap
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    char auColorTable[1024];
};

#pragma pack(pop)

Q_DECLARE_METATYPE( TCell )
Q_DECLARE_METATYPE( TBitMap )

#endif // BITMAP_H
