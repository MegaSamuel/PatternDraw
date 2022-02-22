#include "undostack.h"

//------------------------------------------------------------------------------

TCmdData::TCmdData() : TCmdData(0, 0) {

}

TCmdData::TCmdData(int a_row, int a_col) {
    action = keActionTypeNone;

    row = a_row;
    col = a_col;

    tPrevColor = Qt::white;
    tCurrColor = Qt::white;
}

//------------------------------------------------------------------------------

TUndoStack::TUndoStack() {

}

//------------------------------------------------------------------------------

void  TUndoStack::stInit() {
    // очищаем стеки
    stUndoClean();
    stRedoClean();
}

//------------------------------------------------------------------------------

bool  TUndoStack::isUndoEmpty() const {
    return m_stUndo.empty();
}

bool  TUndoStack::isRedoEmpty() const {
    return m_stRedo.empty();
}

//------------------------------------------------------------------------------

TCmdData  TUndoStack::stUndoTop() {
    if(!m_stUndo.empty())
        return m_stUndo.top();
    return TCmdData();
}

void  TUndoStack::stUndoPop() {
    m_stUndo.pop();
}

void  TUndoStack::stUndoPush(const TCmdData& cmd) {
    m_stUndo.push(cmd);
}

TCmdData  TUndoStack::stRedoTop() {
    if(!m_stRedo.empty())
        return m_stRedo.top();
    return TCmdData();
}

void  TUndoStack::stRedoPop() {
    m_stRedo.pop();
}

void  TUndoStack::stRedoPush(const TCmdData& cmd) {
    m_stRedo.push(cmd);
}

//------------------------------------------------------------------------------

void  TUndoStack::stUndoClean() {
    while(!m_stUndo.empty()) {
        m_stUndo.pop();
    }
}

void  TUndoStack::stRedoClean() {
    while(!m_stRedo.empty()) {
        m_stRedo.pop();
    }
}

//------------------------------------------------------------------------------
