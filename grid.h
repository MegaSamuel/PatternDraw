#ifndef TGRID_H
#define TGRID_H

#include <vector>

#include "element.h"

//------------------------------------------------------------------------------

class TGrid
{
public:
    inline static constexpr int MAX_ROW_COUNT = 1024;
    inline static constexpr int MAX_COLUMN_COUNT = 1024;

    explicit TGrid(int row, int column);

    bool  setRows(int count);
    bool  setColumns(int count);

    int   getRows();
    int   getColumns();

    bool  incRow();
    bool  decRow();

    bool  incColumn();
    bool  decColumn();

private:
    std::vector<std::vector<TElement>> m_grid;

    int   m_row_count;
    int   m_column_count;

    bool  isRowValid(int value);
    bool  isColumnValid(int value);
};

//------------------------------------------------------------------------------

#endif // TGRID_H
