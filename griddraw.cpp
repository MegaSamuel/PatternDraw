#include <QDebug>

#include "griddraw.h"

//------------------------------------------------------------------------------

TGridDraw::TGridDraw(QWidget *parent) : QWidget(parent) {
    m_color = Qt::gray;
}

TGridDraw::~TGridDraw() {

}

//------------------------------------------------------------------------------

/* Метод, в котором происходит рисование
 * */
void  TGridDraw::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this); // Создаём объект отрисовщика

    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    painter.setBrush(QBrush(m_color, Qt::SolidPattern));
    painter.drawEllipse(25, 25, 50, 75);
}

void  TGridDraw::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
//        qDebug() << "press left event";
//        qDebug() << "x =" << event->pos().x() << "y =" << event->pos().y();
        m_color = Qt::green;
        repaint();
    } else if(event->button() == Qt::RightButton) {
//        qDebug() << "press right event";
        m_color = Qt::red;
        repaint();
    }
}

void  TGridDraw::mouseMoveEvent(QMouseEvent *event) {
//    qDebug() << "move event";

    Q_UNUSED(event)
}

void  TGridDraw::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    //qDebug() << "griddraw" << event->size().width() << event->size().height();

    // новые размеры лейбла
//    int w = ui->lblPicture->width();
//    int h = ui->lblPicture->height();
}

//------------------------------------------------------------------------------
