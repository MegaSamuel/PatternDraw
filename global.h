#ifndef GLOBAL_H
#define GLOBAL_H

#include "uni_static.h"
#include "grid.h"

//------------------------------------------------------------------------------

enum EGridType
{
    keGridTypeNormal = 0,
    keGridTypeShift  = 1,
};

enum EItemType
{
    keItemTypeRectan = 0,
    keItemTypeSquare = 1,
};

inline static constexpr int ROW_COUNT = 10;
inline static constexpr int COLUMN_COUNT = 10;

//------------------------------------------------------------------------------

struct TGridData {
    int  nItemType;
    int  nGridType;

    int  nRow;
    int  nColumn;

    explicit TGridData();
    explicit TGridData(int, int, int, int);
};

//------------------------------------------------------------------------------

class TGlobal : public TStatic<TGlobal>
{
public:
    explicit TGlobal();

    TGridData   tGridData;

    QColor      tItemColor;
    QColor      tGridColor;

    TGrid      *pGrid;
};

//------------------------------------------------------------------------------

inline TGlobal&  glb(void)
{   return  TGlobal::get_instance(); }

//------------------------------------------------------------------------------

#endif // GLOBAL_H
