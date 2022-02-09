#include <QDebug>

#include "griddraw.h"
#include "global.h"

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

    qDebug() << __func__ << glb().m_pGrid->getRows() << glb().m_pGrid->getColumns();

    // если border то ставим цвет, иначе цвет как фон
    if(glb().m_pGrid->getBorder())
        painter.setPen(QPen(glb().m_tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    else
        painter.setPen(QPen(glb().m_tElemColor, 1, Qt::SolidLine, Qt::FlatCap));

    painter.setBrush(QBrush(glb().m_tElemColor, Qt::SolidPattern));

    for(int i = 0; i < glb().m_pGrid->getRows(); i++) {
        for(int j = 0; j < glb().m_pGrid->getColumns(); j++) {
            int w = 10;
            int h = 10;
            int x = 0 + j*w;
            int y = 0 + i*h;
            painter.drawRect(x, y, w, h);
        }
    }

    // Устанавливаем кисть абриса
//    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
//    painter.setBrush(QBrush(m_color, Qt::SolidPattern));
//    painter.drawEllipse(25, 25, 50, 75);
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
