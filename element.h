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

    // это скорее всего тут ненужно
    void          setBorder(bool border = true);
    void          setBorderColor(const QColor& color = Qt::gray);

    // это скорее всего тут ненужно
    bool          getBorder() const;
    QColor        getBorderColor() const;

    void          setFill(bool fill = true);
    void          setFillColor(const QColor& color = Qt::white);

    bool          getFill() const;
    QColor        getFillColor() const;

    // это скорее всего тут ненужно
    void          setSplit(bool split = true);
    bool          getSplit();

private:
    std::pair<int, int>  m_id;

    std::string   m_text;
    QColor        m_text_color;

    bool          m_border;
    QColor        m_border_color;

    bool          m_fill;
    QColor        m_fill_color;

    bool          m_split;
};

//------------------------------------------------------------------------------

#endif // TELEMENT_H
