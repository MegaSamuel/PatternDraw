#ifndef TGRID_H
#define TGRID_H

#include <vector>

#include "element.h"

//------------------------------------------------------------------------------

class TGrid
{
public:
    inline static constexpr int MAX_ROW_COUNT = 128;
    inline static constexpr int MAX_COLUMN_COUNT = 128;

    explicit TGrid(int row, int column);
    explicit TGrid(int row, int column, int row_max, int column_max);

    bool          setRows(int count);
    bool          setColumns(int count);

    int           getRows();
    int           getColumns();

    bool          incRow();
    bool          decRow();

    bool          incColumn();
    bool          decColumn();

    void          setBorder(bool border);
    bool          getBorder() const;

private:
    std::vector<std::vector<TElement>> m_grid;

    int           m_row_count;
    int           m_column_count;

    int           m_max_row_count;
    int           m_max_column_count;

    bool          m_border;

    bool          isRowValid(int value);
    bool          isColumnValid(int value);
};

//------------------------------------------------------------------------------

#endif // TGRID_H
