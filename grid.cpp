#include <cassert>

#include "grid.h"

//------------------------------------------------------------------------------

TGrid::TGrid(int row, int column) {
    assert(isRowValid(row));
    assert(isColumnValid(column));

    m_row_count = row;
    m_column_count = column;

    m_grid.resize(static_cast<unsigned>(m_row_count));

    for(int i = 0; i < m_row_count; i++) {
        m_grid[static_cast<unsigned>(i)].resize(static_cast<unsigned>(m_column_count));
    }

    for(int i = 0; i < m_row_count; i++) {
        for(int j = 0; j < m_column_count; j++) {
            m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setId(i+1, j+1);
        }
    }
}

//------------------------------------------------------------------------------

bool  TGrid::setRows(int count) {
    if(isRowValid(count)) {
        m_row_count = count;
        return true;
    }
    return false;
}

bool  TGrid::setColumns(int count) {
    if(isColumnValid(count)) {
        m_column_count = count;
        return true;
    }
    return false;
}

int   TGrid::getRows() {
    return m_row_count;
}

int   TGrid::getColumns() {
    return m_column_count;
}

bool  TGrid::incRow() {
    if(isRowValid(m_row_count+1)) {
        ++m_row_count;
        return true;
    }
    return false;
}

bool  TGrid::decRow() {
    if(isRowValid(m_row_count-1)) {
        --m_row_count;
        return true;
    }
    return false;
}

bool  TGrid::incColumn() {
    if(isRowValid(m_column_count+1)) {
        ++m_column_count;
        return true;
    }
    return false;
}

bool  TGrid::decColumn() {
    if(isRowValid(m_column_count-1)) {
        --m_column_count;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

bool  TGrid::isRowValid(int value) {
    return ((0 < value) && (value < MAX_ROW_COUNT));
}

bool  TGrid::isColumnValid(int value) {
    return ((0 < value) && (value < MAX_COLUMN_COUNT));
}

//------------------------------------------------------------------------------
