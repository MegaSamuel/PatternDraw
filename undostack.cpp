#include "undostack.h"

//------------------------------------------------------------------------------

TUndoStack::TUndoStack()
{

}

//------------------------------------------------------------------------------

void  TUndoStack::stInit() {
    // очищаем стек
    while(!m_stRedo.empty()) {
        m_stRedo.pop();
    }

    // очищаем стек
    while(!m_stUndo.empty()) {
        m_stUndo.pop();
    }
}

int  TUndoStack::stUndoTop() {
    if(!m_stUndo.empty())
        return m_stUndo.top();
    return -1;
}

void  TUndoStack::stUndoPop() {
    m_stUndo.pop();
}

int  TUndoStack::stRedoTop() {
    if(!m_stRedo.empty())
        return m_stRedo.top();
    return -1;
}

void  TUndoStack::stRedoPop() {
    m_stRedo.pop();
}

int  TUndoStack::doUndo() {
    return -1;
}

int  TUndoStack::doRedo() {
    if(m_stRedo.empty())
        return -1;

    // забрали команду из redo
    int cmd = m_stRedo.top();
    m_stRedo.pop();

    // положили команду в undo
    m_stUndo.push(cmd);

    // выполняем команду
    return cmd;
}

//------------------------------------------------------------------------------
