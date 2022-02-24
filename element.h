#ifndef TELEMENT_H
#define TELEMENT_H

#include <QtWidgets>

#include <string>

//------------------------------------------------------------------------------

class TElement
{
public:
    explicit      TElement();
    explicit      TElement(std::pair<int, int> id);

    void                setId(std::pair<int, int> id);
    std::pair<int, int> getId() const;

    void          setText(const std::string& txt = "");
    void          setTextColor(const QColor& color = Qt::black);

    std::string   getText() const;
    QColor        getTextColor() const;

    void          setFill(bool fill = true);
    void          setFillColor(const QColor& color = Qt::white);

    bool          getFill() const;
    QColor        getFillColor() const;

    void          setBackFill(bool fill = true);
    void          setBackFillColor(const QColor& color = Qt::white);

    bool          getBackFill() const;
    QColor        getBackFillColor() const;

private:
    std::pair<int, int>  m_id;

    std::string   m_text;
    QColor        m_text_color;

    bool          m_fill;
    QColor        m_fill_color;

    bool          m_back_fill;
    QColor        m_back_fill_color;
};

//------------------------------------------------------------------------------

#endif // TELEMENT_H
