#ifndef TUNDOSTACK_H
#define TUNDOSTACK_H

#include <QColor>

#include <stack>

//! ->
//!
//! TODO нужно добавить ограничение на количество запоминаемых шагов
//!      возможно уйти от std::stack на std::deque
//! <-

//------------------------------------------------------------------------------

enum EActionType
{
    keActionTypeNone     = 0,
    keActionTypeColor    = 1,
};

struct TElemData {
    bool    elem_fill;
    QColor  elem_color;

    bool    back_fill;
    QColor  back_color;

    explicit TElemData();
};

struct TCmdData {
     // тип события
    int     action;

    // для какой ячейки
    int     row;
    int     col;

    // предыдущие и новые данные
    TElemData  tPrevData;
    TElemData  tCurrData;

    explicit TCmdData();
    explicit TCmdData(int, int);
};

//------------------------------------------------------------------------------

class TUndoStack
{
public:
    TUndoStack();

    void   stInit();

    // проверка наличия запомненных действий
    bool        isUndoEmpty() const;
    bool        isRedoEmpty() const;

    // доступ к стеку undo
    TCmdData    stUndoTop();
    void        stUndoPop();
    void        stUndoPush(const TCmdData& cmd);

    // доступ к стеку redo
    TCmdData    stRedoTop();
    void        stRedoPop();
    void        stRedoPush(const TCmdData& cmd);

    // очистка
    void        stUndoClean();
    void        stRedoClean();

private:
    std::stack<TCmdData>  m_stUndo;
    std::stack<TCmdData>  m_stRedo;
};

//------------------------------------------------------------------------------

#endif // TUNDOSTACK_H
