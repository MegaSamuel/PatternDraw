#include "element.h"

//------------------------------------------------------------------------------

TElement::TElement() : TElement({0, 0}) {

}

TElement::TElement(std::pair<int, int> id) {
    setId(id);

    m_text.clear();
    m_text_color = Qt::black;

    m_border = true;
    m_border_color = Qt::gray;

    m_fill = false;
    m_fill_color = Qt::white;

    m_split = false;
}

//------------------------------------------------------------------------------

void TElement::setId(std::pair<int, int> id) {
    m_id = id;
}

std::pair<int, int> TElement::getId() const {
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

void TElement::setSplit(bool split) {
    m_split = split;
}

bool TElement::getSplit() {
    return m_split;
}

//------------------------------------------------------------------------------
