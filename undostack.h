#ifndef TUNDOSTACK_H
#define TUNDOSTACK_H

#include <stack>

//------------------------------------------------------------------------------

class TUndoStack
{
public:
    TUndoStack();

    void   stInit();

    // доступ к стеку undo
    int    stUndoTop();
    void   stUndoPop();

    // доступ к стеку redo
    int    stRedoTop();
    void   stRedoPop();

    // команды на выполнение redo/undo
    int    doUndo();
    int    doRedo();

private:
    std::stack<int>  m_stUndo;
    std::stack<int>  m_stRedo;
};

//------------------------------------------------------------------------------

#endif // TUNDOSTACK_H
