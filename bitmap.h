#ifndef BITMAP_H
#define BITMAP_H

#include "windows.h"

#pragma pack(push, 1)

struct BitMap
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

#pragma pack(pop)

#endif // BITMAP_H
