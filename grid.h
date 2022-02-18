#ifndef TGRID_H
#define TGRID_H

#include <vector>

#include "element.h"
#include "undostack.h"

//------------------------------------------------------------------------------

class TGrid
{
public:
    explicit TGrid(int row, int column);
    explicit TGrid(int row, int column, int row_max, int column_max);

    template <typename ValType>
    bool     setRows(ValType count) {
        if(isRowValid(count)) {
            m_row_count = count;
            return true;
        }
        return false;
    }

    template <typename ValType>
    bool     setColumns(ValType count){
        if(isColumnValid(count)) {
            m_column_count = count;
            return true;
        }
        return false;
    }

    int           getRows();
    int           getColumns();

    bool          incRow();
    bool          decRow();

    bool          incColumn();
    bool          decColumn();

    void          setBorder(bool border);
    bool          getBorder() const;

    void          setColor(int row, int col, QColor color, bool undo = true);
    QColor        getColor(int row, int col) const;

    void          setRulerBorder(bool border);
    bool          getRulerBorder() const;

    void          setSplit(bool split);
    bool          getSplit() const;

    void          setRulerV(bool ruler);
    void          setRulerVtype(int type);
    bool          getRulerV() const;
    int           getRulerVtype() const;

    void          setRulerH(bool ruler);
    void          setRulerHtype(int type);
    bool          getRulerH() const;
    int           getRulerHtype() const;

    const TElement& getElement(int row, int column) const;

    void          initCells();

    bool          doUndo();
    bool          doRedo();

private:
    std::vector<std::vector<TElement>> m_grid;

    int           m_row_count;
    int           m_column_count;

    int           m_max_row_count;
    int           m_max_column_count;

    bool          m_border;
    bool          m_ruler_border;

    bool          m_split;

    bool          m_ruler_v;
    bool          m_ruler_h;

    int           m_ruler_v_type;
    int           m_ruler_h_type;

    bool          isRowValid(int value);
    bool          isColumnValid(int value);

    TUndoStack    m_stUndoRedo;
};

//------------------------------------------------------------------------------

#endif // TGRID_H
