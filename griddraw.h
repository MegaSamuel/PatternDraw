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

//    QImage        *getImage();

    bool           saveImage(const QString &fileName, const char *format = nullptr);

protected:
    /* Определяем виртуальный метод родительского класса
     * для отрисовки содержимого виджета
     * */
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    QColor         m_color;

    QSize          m_hruler_size;
    QSize          m_vruler_size;

    QImage         m_image;

    void           updateRulerSize();

    QSize          getElemSize();
    QPoint         getElemShift();

    void           drawAll(QPainter *painter);
    void           drawPicture();

    // x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawVRuler(int x, int y, ERowNumber number, QPainter *painter);

    // ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawVRulerElement(int ind, int x, int y, QPainter *painter);

    // x, y - начальные координаты; number - нумерация; painter - указатель на отрисовщик
    void DrawHRuler(int x, int y, ERowNumber number, QPainter *painter);

    // ind - номер ячейки; x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawHRulerElement(int ind, int x, int y, QPainter *painter);

    // x, y - начальные координаты; painter - указатель на отрисовщик
    void DrawElements(int x, int y, QPainter *painter);

    // i, j - номер ячейки; x, y - координаты; painter - указатель на отрисовщик
    void DrawElement(int i, int j, int x, int y, QPainter *painter);
};

//------------------------------------------------------------------------------

#endif // TGRIDDRAW_H
