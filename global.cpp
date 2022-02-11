#include "global.h"

//------------------------------------------------------------------------------

TGlobal::TGlobal()
{
    m_uRow = 10;
    m_uColumn = 10;

    m_uItemType = 0;
    m_uGridType = 0;

    m_tItemColor = Qt::white;
    m_tGridColor = Qt::gray;

//    m_uTemp = 0;

    m_pGrid = nullptr;
}

//------------------------------------------------------------------------------
