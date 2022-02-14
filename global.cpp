#include "global.h"

//------------------------------------------------------------------------------

TGridData::TGridData() : TGridData(keItemTypeRectan, keGridTypeShift, ROW_COUNT, COLUMN_COUNT) {

}

TGridData::TGridData(int itype, int gtype, int row, int col) {
    nItemType = itype;
    nGridType = gtype;

    nRow = row;
    nColumn = col;
}

//------------------------------------------------------------------------------

TGlobal::TGlobal()
{
    tItemColor = Qt::white;
    tGridColor = Qt::gray;

    pGrid = nullptr;
}

//------------------------------------------------------------------------------
