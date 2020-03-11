#ifndef BITMAP_H
#define BITMAP_H

#include "windows.h"

// структура ячейки
typedef struct t_cell
{
    unsigned h; // высота ячейки
    unsigned w; // ширина ячейки
}TCell;

#pragma pack(push, 1)

struct BitMap
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    char auColorTable[1024];
};

#pragma pack(pop)

#endif // BITMAP_H
