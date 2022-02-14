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

enum ERulerType
{
    keRulerTypeRight     = 0,
    keRulerTypeBottom    = 0,
    keRulerTypeRightLeft = 1,
    keRulerTypeBottomTop = 1,
};

inline static constexpr int ROW_COUNT = 10;
inline static constexpr int COLUMN_COUNT = 10;

inline static constexpr int MIN_ROW_COUNT = 1;
inline static constexpr int MIN_COLUMN_COUNT = 1;

inline static constexpr int MAX_ROW_COUNT = 128;
inline static constexpr int MAX_COLUMN_COUNT = 128;

inline static constexpr int SHORT_SIDE = 20;
inline static constexpr int LONG_SIDE = 30;

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
