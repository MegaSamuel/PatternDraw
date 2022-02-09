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

public:
    explicit TGridDraw(QWidget *parent = nullptr);
    explicit TGridDraw(TGrid *grid, QWidget *parent = nullptr);
    ~TGridDraw() override;

protected:
    /* Определяем виртуальный метод родительского класса
     * для отрисовки содержимого виджета
     * */
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    TGrid         *m_pGrid;
    QColor         m_color;
};

//------------------------------------------------------------------------------

#endif // TGRIDDRAW_H
