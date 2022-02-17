#ifndef TGRIDDRAW_H
#define TGRIDDRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <grid.h>

//------------------------------------------------------------------------------

namespace Ui {
    class TGridDraw;
}

class TGridDraw : public QWidget
{
    Q_OBJECT

    enum ERowNumber
    {
        keRowNumberNone = 0,
        keRowNumberAll  = 1,
        keRowNumberOdd  = 2,
        keRowNumberEven = 3,
    };

public:
    explicit TGridDraw(QWidget *parent = nullptr);
    ~TGridDraw() override;

    bool           saveImage(const QString &fileName, const char *format = nullptr);

    int            getCurrRow() const;
    int            getCurrColumn() const;

Q_SIGNALS:
    void           currentPos(int, int);
    void           changeState();

protected:
    /* Определяем виртуальный метод родительского класса
     * для отрисовки содержимого виджета
     * */
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    QSize          m_hruler_size;
    QSize          m_vruler_size;

    QImage         m_image;

    void           updateRulerSize();

    QSize          getElemSize();
    QPoint         getElemShift();

    void           drawAll(QPainter *painter);
    void           drawPicture();

    //! координаты левого верхнего угла таблицы
    QPoint         m_left_top_point;
    //! координаты правого ниждего угла таблицы
    QPoint         m_right_bottom_point;

    //! размер всей картинки (с учеиом линеек)
    QSize          m_pic_size;

    //! расчет номера ряда
    int            calcRowNum(int y);
    //! расчет номера петли
    int            calcColumnNum(int x);

    //! текущий номера ряда
    int            m_curr_row;
    //! текущий номер петли
    int            m_curr_column;

    //! предыдущий номера ряда
    int            m_prev_row;
    //! предыдущий номер петли
    int            m_prev_column;

    //! x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawVRuler(int x, int y, ERowNumber number, QPainter *painter);

    //! ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawVRulerElement(int ind, int x, int y, QPainter *painter);

    //! x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawHRuler(int x, int y, ERowNumber number, QPainter *painter);

    //! ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawHRulerElement(int ind, int x, int y, QPainter *painter);

    //! x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawElements(int x, int y, QPainter *painter);

    //! i, j - номер ячейки; x, y - координаты; painter - указатель на отрисовщик
    void DrawElement(int i, int j, int x, int y, QPainter *painter);

    template<typename T>
    bool isEven(T value) const {
        return !(std::abs(value)%2);
    }

    template<typename T>
    bool isOdd(T value) const {
        return (std::abs(value)%2);
    }
};

//------------------------------------------------------------------------------

#endif // TGRIDDRAW_H
