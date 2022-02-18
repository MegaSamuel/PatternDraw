#include <cassert>

#include "grid.h"
#include "global.h"

//------------------------------------------------------------------------------

TGrid::TGrid(int row, int column) : TGrid(row, column, MAX_ROW_COUNT, MAX_COLUMN_COUNT) {

}

TGrid::TGrid(int row, int column, int row_max, int column_max) {
    m_row_count = row;
    m_column_count = column;

    // тут бы std::clamp использовать...
    if((MIN_ROW_COUNT <= row_max) && (row_max <= MAX_ROW_COUNT))
        m_max_row_count = row_max;
    else
        m_max_row_count = MAX_ROW_COUNT;

    if((MIN_COLUMN_COUNT <= column_max) && (column_max <= MAX_COLUMN_COUNT))
        m_max_column_count = column_max;
    else
        m_max_column_count = MAX_COLUMN_COUNT;

    assert(isRowValid(row));
    assert(isColumnValid(column));

    m_border = true;
    m_ruler_border = true;
    m_split = false;

    m_ruler_v = false;
    m_ruler_h = false;

    m_ruler_v_type = keRulerTypeRight;
    m_ruler_h_type = keRulerTypeBottom;

    m_grid.resize(static_cast<unsigned>(m_max_row_count));
    for(int i = 0; i < m_max_row_count; i++) {
        m_grid[static_cast<unsigned>(i)].resize(static_cast<unsigned>(m_max_column_count));
    }

    for(int i = 0; i < m_row_count; i++) {
        for(int j = 0; j < m_column_count; j++) {
            m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setId({i+1, j+1});
        }
    }
}

void  TGrid::initCells() {
    // инициализация ячеек
    for(int i = 0; i < m_row_count; i++) {
        for(int j = 0; j < m_column_count; j++) {
            m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setFill(false);
            m_grid[static_cast<unsigned>(i)][static_cast<unsigned>(j)].setFillColor(Qt::white);
        }
    }

    // инициализация undo/redo
    m_stUndoRedo.stInit();
}

//------------------------------------------------------------------------------

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

void  TGrid::setColor(int row, int col, QColor color, bool undo) {
    TElement& elem = m_grid.at(static_cast<unsigned>(row)).at(static_cast<unsigned>(col));

    if(undo) {
        TCmdData  cmd(row, col);
        // запоминаем, что было и что станет
        cmd.action = keActionTypeColor;
        cmd.tCurrColor = color;
        cmd.tPrevColor = elem.getFillColor();
        // добавляем в undo
        m_stUndoRedo.stUndoPush(cmd);

        // получили новую команду от оператора - очищаем redo
        m_stUndoRedo.stRedoClean();
    }

    // применяем действие к ячейке
    elem.setFill(true);
    elem.setFillColor(color);
}

QColor  TGrid::getColor(int row, int col) const {
    const TElement& elem = m_grid.at(static_cast<unsigned>(row)).at(static_cast<unsigned>(col));
    return elem.getFillColor();
}

void  TGrid::setSplit(bool split) {
    m_split = split;
}

void  TGrid::setRulerBorder(bool border) {
    m_ruler_border = border;
}

bool TGrid::getRulerBorder() const {
    return m_ruler_border;
}

bool TGrid::getSplit() const {
    return m_split;
}

void  TGrid::setRulerV(bool ruler) {
    m_ruler_v = ruler;
}

void  TGrid::setRulerVtype(int type) {
    m_ruler_v_type = type;
}

bool TGrid::getRulerV() const {
    return m_ruler_v;
}

int TGrid::getRulerVtype() const {
    return m_ruler_v_type;
}

void  TGrid::setRulerH(bool ruler) {
    m_ruler_h = ruler;
}

void  TGrid::setRulerHtype(int type) {
    m_ruler_h_type = type;
}

bool TGrid::getRulerH() const {
    return m_ruler_h;
}

int TGrid::getRulerHtype() const {
    return m_ruler_h_type;
}

//------------------------------------------------------------------------------

bool  TGrid::doUndo() {
    bool result = false;

    if(m_stUndoRedo.isUndoEmpty())
        return result;

    qDebug() << "undo";

    // забираем cmd из undo
    TCmdData cmd = m_stUndoRedo.stUndoTop();
    m_stUndoRedo.stUndoPop();

    // кладем cmd в redo
    m_stUndoRedo.stRedoPush(cmd);

    if(keActionTypeColor == cmd.action) {
        // меняем цвет, это изменение в undo не записываем (4-й агрумент false)
        setColor(cmd.row, cmd.col, cmd.tPrevColor, false);
        result = true;
    }

    return result;
}

bool  TGrid::doRedo() {
    bool result = false;

    if(m_stUndoRedo.isRedoEmpty())
        return result;

    qDebug() << "redo";

    // забираем cmd из undo
    TCmdData cmd = m_stUndoRedo.stRedoTop();
    m_stUndoRedo.stRedoPop();

    // кладем cmd в undo (напрямую, не через фунцию (setColor))
    m_stUndoRedo.stRedoPush(cmd);

    if(keActionTypeColor == cmd.action) {
        // меняем цвет, это изменение в undo уже записано (4-й агрумент false)
        setColor(cmd.row, cmd.col, cmd.tCurrColor, false);
        result = true;
    }

    return result;
}

//------------------------------------------------------------------------------

bool  TGrid::isRowValid(int value) {
    return ((0 < value) && (value <= m_max_row_count));
}

bool  TGrid::isColumnValid(int value) {
    return ((0 < value) && (value <= m_max_column_count));
}

//------------------------------------------------------------------------------

const TElement&  TGrid::getElement(int row, int column) const {
    const TElement& elem = m_grid.at(static_cast<unsigned>(row)).at(static_cast<unsigned>(column));
    return elem;
}

//------------------------------------------------------------------------------
