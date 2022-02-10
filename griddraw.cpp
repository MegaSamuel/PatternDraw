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

    //qDebug() << __func__ << glb().m_pGrid->getRows() << glb().m_pGrid->getColumns();

    int x_shift = 0;
    QSize  elem_size = getElemSize();

    if(glb().m_pGrid->getRuler()) {
        DrawRuler(x_shift, 0, &painter);
        x_shift += elem_size.width();
    }

    DrawElements(x_shift, 0, &painter);
}

void  TGridDraw::DrawRuler(int x, int y, QPainter *painter) {
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(painter)
}

void  TGridDraw::DrawElements(int x, int y, QPainter *painter) {
    int _x = 0;
    int _y = 0;

    QSize  elem_size = getElemSize();
    QPoint elem_shift = getElemShift();

    for(int i = 0; i < glb().m_pGrid->getRows(); i++) {
        for(int j = 0; j < glb().m_pGrid->getColumns(); j++) {
            if(keGridTypeNormal == glb().m_uGridType) {
                _x = x + elem_shift.x() + j*elem_size.width();
                _y = y + elem_shift.y() + i*elem_size.height();
                DrawElement(i, j, _x, _y, painter);
            } else if(keGridTypeShift == glb().m_uGridType) {
                if(i%2) {
                    if(j%2) {
                        // четная строка, четный столбец
                        _x = x + j*elem_size.width();
                        _y = y + i*elem_shift.y();
                        DrawElement(i, j, _x, _y, painter);
                    }
                } else {
                    if(!(j%2)) {
                        // нечетная строка, нечетный столбец
                        _x = x + j*elem_size.width();
                        _y = y + i*elem_shift.y();
                        DrawElement(i, j, _x, _y, painter);
                    }
                }
            }
        }
    }
}

void  TGridDraw::DrawElement(int i, int j, int x, int y, QPainter *painter) {
    QSize  elem_size = getElemSize();

    // если border то ставим цвет, иначе цвет как фон
    if(glb().m_pGrid->getBorder()) {
        painter->setPen(QPen(glb().m_tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    } else {
        if(glb().m_pGrid->getElement(i,j).getFill())
            painter->setPen(QPen(glb().m_tItemColor, 1, Qt::SolidLine, Qt::FlatCap));
        else
            painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    if(glb().m_pGrid->getElement(i,j).getFill())
        painter->setBrush(QBrush(glb().m_tItemColor, Qt::SolidPattern));
    else
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

    painter->drawRect(x, y, elem_size.width(), elem_size.height());
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

QSize  TGridDraw::getElemSize() {
    QSize size(10, 10);

    if(keItemTypeRectan == glb().m_uItemType) {
        size.setHeight(20);
        size.setWidth(10);
    } else if(keItemTypeSquare == glb().m_uItemType) {
        size.setHeight(10);
        size.setWidth(10);
    }

    return size;
}

QPoint  TGridDraw::getElemShift() {
    QPoint shift(0, 0);

    if(keGridTypeNormal == glb().m_uGridType) {
        shift.setX(0);
        shift.setY(0);
    } else if(keGridTypeShift == glb().m_uGridType) {
        shift.setX(0);
        shift.setY(getElemSize().height()/2);
    }

    return shift;
}

//------------------------------------------------------------------------------
