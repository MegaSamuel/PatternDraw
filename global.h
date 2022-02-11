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

enum EItemSize
{
    keItemSizeSmall  = 0,
    keItemSizeNormal = 1,
    keItemSizeHuge   = 2,
};

class TGlobal : public TStatic<TGlobal>
{
public:
    explicit TGlobal();

    unsigned       m_uItemType;
    unsigned       m_uGridType;

    QColor         m_tItemColor;
    QColor         m_tGridColor;

//    unsigned       m_uTemp;

    TGrid         *m_pGrid;
};

//------------------------------------------------------------------------------

inline TGlobal&  glb(void)
{   return  TGlobal::get_instance(); }

//------------------------------------------------------------------------------

#endif // GLOBAL_H
