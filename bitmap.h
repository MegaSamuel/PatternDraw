#ifndef BITMAP_H
#define BITMAP_H

#include "windows.h"

struct BitMap
{
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

#endif // BITMAP_H
