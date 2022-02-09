#include <cassert>

#include "grid.h"

//------------------------------------------------------------------------------

TGrid::TGrid(int row, int column) : TGrid(row, column, MAX_ROW_COUNT, MAX_COLUMN_COUNT) {

}

TGrid::TGrid(int row, int column, int row_max, int column_max) {
    m_row_count = row;
    m_column_count = column;

    if(1 > m_max_row_count)
        m_max_row_count = MAX_ROW_COUNT;
    else
        m_max_row_count = row_max;

    if(1 > m_max_column_count)
        m_max_column_count = MAX_COLUMN_COUNT;
    else
        m_max_column_count = column_max;

    assert(isRowValid(row));
    assert(isColumnValid(column));

    m_border = true;

    m_grid.resize(static_cast<unsigned>(m_max_row_count));
    for(int i = 0; i < m_max_row_count; i++) {
        m_grid[static_cast<unsigned>(i)].resize(static_cast<unsigned>(m_max_column_count));
    }

    for(int i = 0; i < m_row_count; i++) {
        for(int j = 0; j < m_column_count; j++) {
            m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setId({i+1, j+1});
        }
    }

//    qDebug() << "sizeof grid" << static_cast<unsigned>(m_max_row_count*m_max_column_count)*sizeof(TElement);
//    qDebug() << "sizeof grid" << m_grid.capacity();
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

void  TGrid::setBorder(bool border) {
//    qDebug() << "try set border" << border << "current border" << m_border;

    if(border != m_border) {
        m_border = border;
        for(int i = 0; i < m_row_count; i++) {
            for(int j = 0; j < m_column_count; j++) {
                m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setBorder(m_border);
            }
        }
    }
}

bool TGrid::getBorder() const {
    return m_border;
}

//------------------------------------------------------------------------------

bool  TGrid::isRowValid(int value) {
    return ((0 < value) && (value < m_max_row_count));
}

bool  TGrid::isColumnValid(int value) {
    return ((0 < value) && (value < m_max_column_count));
}

//------------------------------------------------------------------------------
