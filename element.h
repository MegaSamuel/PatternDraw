#ifndef TELEMENT_H
#define TELEMENT_H

#include <QtWidgets>

#include <string>

//------------------------------------------------------------------------------

class TElement
{
public:
    explicit      TElement();
    explicit      TElement(int row, int column);

    void          setId(int row, int column);
    uint64_t      getId() const;

    void          setText(const std::string& txt = "");
    void          setTextColor(const QColor& color = Qt::black);

    std::string   getText() const;
    QColor        getTextColor() const;

    void          setBorder(bool border = true);
    void          setBorderColor(const QColor& color = Qt::gray);

    bool          getBorder() const;
    QColor        getBorderColor() const;

    void          setFill(bool fill = true);
    void          setFillColor(const QColor& color = Qt::white);

    bool          getFill() const;
    QColor        getFillColor() const;

private:
    uint64_t      m_id;

    std::string   m_text;
    QColor        m_text_color;

    bool          m_border;
    QColor        m_border_color;

    bool          m_fill;
    QColor        m_fill_color;
};

//------------------------------------------------------------------------------

#endif // TELEMENT_H
