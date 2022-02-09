#ifndef GLOBAL_H
#define GLOBAL_H

#include "uni_static.h"
#include "grid.h"

//------------------------------------------------------------------------------

class TGlobal : public TStatic<TGlobal>
{
public:
    explicit TGlobal();

    unsigned       m_uItemType;
    unsigned       m_uItemSize;
    unsigned       m_uGridType;

    unsigned       m_uTemp;

    TGrid         *m_pGrid;
};

//------------------------------------------------------------------------------

inline TGlobal&  glb(void)
{   return  TGlobal::get_instance(); }

//------------------------------------------------------------------------------

#endif // GLOBAL_H
