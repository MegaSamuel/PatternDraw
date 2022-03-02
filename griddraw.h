#ifndef TGRIDDRAW_H
#define TGRIDDRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "grid.h"

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
    bool           saveImageConverted(const QString &fileName, const char *format = nullptr);
    QImage*        getImage();

    int            getCurrRow() const;
    int            getCurrColumn() const;

Q_SIGNALS:
    void           currentPos(int, int);
    void           changeState();

public Q_SLOTS:
    void       onChangeGridColor(QColor);
    void       onChangeBackColor(QColor);

protected:
    /* Определяем виртуальный метод родительского класса
     * для отрисовки содержимого виджета
     * */
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    //! размер ячейки горизонтальной линейки
    QSize          m_hruler_size;
    //! размер ячейки вертикальной линейки
    QSize          m_vruler_size;

    //! картинка с сеткой
    QImage         m_image;
    //! конвертированная картинка с сеткой
    QImage         m_image_converted;

    //! пересчитать размер ячеек линеек
    void           updateRulerSize();

    //! вернуть размер ячейки сетки
    QSize          getElemSize();
    //! вернуть смещение ячейки сетки
    QPoint         getElemShift();

    //! нарисовать сетку
    void           drawAll(QPainter *painter, bool converted = false);
    //! нарисовать сетку в картинку (m_image/m_image_converted)
    void           drawPicture(bool converted = false);

    //! координаты левого верхнего угла таблицы
    QPoint         m_left_top_point;
    //! координаты правого ниждего угла таблицы
    QPoint         m_right_bottom_point;

    //! размер всей картинки (с учетом линеек)
    QSize          m_pic_size;
    QSize          m_pic_size_converted;

    //! расчет номера ряда
    int            calcRowNum(int y);
    //! расчет номера петли
    int            calcColumnNum(int x);

    //! текущий номера ряда
    int            m_curr_row;
    //! текущий номер петли (справа налево)
    int            m_curr_column;

    //! предыдущий номера ряда
    int            m_prev_row;
    //! предыдущий номер петли (справа налево)
    int            m_prev_column;

    //! необходимость отправить сигнал с признаком отсутствия номера текучей ячейки (курсор вне таблицы)
    bool           m_need_to_emit;

    void DrawGrid(int x, int y, QPainter *painter);

    //! x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawVRuler(int x, int y, ERowNumber number, QPainter *painter);

    //! ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawVRulerElement(int ind, int x, int y, QPainter *painter);

    //! x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawVRulerAdv(int x, int y, ERowNumber number, QPainter *painter);

    //! ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawVRulerAdvElement(int ind, int x, int y, QPainter *painter);

    //! x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawHRuler(int x, int y, ERowNumber number, QPainter *painter);

    //! ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawHRulerElement(int ind, int x, int y, QPainter *painter);

    //! x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawElements(int x, int y, QPainter *painter, bool converted);

    //! i, j - номер ячейки; x, y - координаты; painter - указатель на отрисовщик
    void DrawElement(int i, int j, int x, int y, QPainter *painter, bool converted);

    //! возврат истины если модуль числа четный
    template<typename T>
    bool isEven(T value) const {
        return !(std::abs(value)%2);
    }

    //! возврат истины если модуль числа нечетный
    template<typename T>
    bool isOdd(T value) const {
        return (std::abs(value)%2);
    }
};

//------------------------------------------------------------------------------

#endif // TGRIDDRAW_H
