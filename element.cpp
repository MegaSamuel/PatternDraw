#include "element.h"

//------------------------------------------------------------------------------

TElement::TElement(int id) : m_id(id) {
    m_text.clear();
    m_text_color = Qt::black;

    m_border = false;
    m_border_color = Qt::gray;

    m_fill = false;
    m_fill_color = Qt::white;
}

//------------------------------------------------------------------------------

void TElement::setId(int id) {
    m_id = id;
}

int TElement::getId() const {
    return m_id;
}

void TElement::setText(const std::string& txt) {
    m_text = txt;
}

void TElement::setTextColor(const QColor& color) {
    m_text_color = color;
}

std::string TElement::getText() const {
    return m_text;
}

QColor TElement::getTextColor() const {
    return m_text_color;
}

void TElement::setBorder(bool border) {
    m_border = border;
}

void TElement::setBorderColor(const QColor& color) {
    m_border_color = color;
}

bool TElement::getBorder() const {
    return m_border;
}

QColor TElement::getBorderColor() const {
    return m_border_color;
}

void TElement::setFill(bool fill) {
    m_fill = fill;
}

void TElement::setFillColor(const QColor& color) {
    m_fill_color = color;
}

bool TElement::getFill() const {
    return m_fill;
}

QColor TElement::getFillColor() const {
    return m_fill_color;
}

//------------------------------------------------------------------------------
