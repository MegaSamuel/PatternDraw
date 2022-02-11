#include <QDebug>

#include "griddraw.h"
#include "global.h"

//------------------------------------------------------------------------------

TGridDraw::TGridDraw(QWidget *parent) : QWidget(parent) {
    m_color = Qt::gray;

    m_hruler_size.setHeight(10);
    m_hruler_size.setWidth(10);

    m_vruler_size.setHeight(10);
    m_vruler_size.setWidth(10);
}

TGridDraw::~TGridDraw() {

}

//------------------------------------------------------------------------------

void  TGridDraw::updateRulerSize() {
    QSize  elem_size = getElemSize();

    m_hruler_size.setHeight(20);
    m_hruler_size.setWidth(elem_size.width());

    m_vruler_size.setHeight(elem_size.height());
    m_vruler_size.setWidth(20);
}

//------------------------------------------------------------------------------

/* Метод, в котором происходит рисование
 * */
void  TGridDraw::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this); // Создаём объект отрисовщика

    int x_shift = 0;
    int y_shift = 0;

    int y_shift_start = 0;

    updateRulerSize();

    // рулетка слева
    if(glb().pGrid->getRuler()) {
        DrawVRuler(x_shift, y_shift, keRowNumberEven, &painter);
        // сдвигаемся на ширину вертикальной рулетки
        x_shift += m_vruler_size.width();
    }

    // собственно табличка ячеек
    DrawElements(x_shift, y_shift, &painter);
    QSize  elem_size = getElemSize();
    // сдвигаемся на высоту таблицы
    y_shift += glb().pGrid->getRows()*elem_size.height();

    // рулетка снизу
    if(glb().pGrid->getRuler()) {
        DrawHRuler(x_shift, y_shift, keRowNumberAll, &painter);
    }

    // сдвигаемся на ширину таблицы
    x_shift += glb().pGrid->getColumns()*elem_size.width();

    // рулетка справа
    if(glb().pGrid->getRuler()) {
        DrawVRuler(x_shift, y_shift_start, keRowNumberOdd, &painter);
        // сдвигаемся на ширину вертикальной рулетки
        x_shift += m_vruler_size.width();
    }
}

void  TGridDraw::DrawVRuler(int x, int y, ERowNumber number, QPainter *painter) {
    int _x = 0;
    int _y = 0;

    _x += x;

    QSize  elem_size = getElemSize();
    QPoint elem_shift = getElemShift();

    int num_for_draw = -1;

    for(int i = 0; i < glb().pGrid->getRows(); i++) {
        if(keGridTypeNormal == glb().tGridData.nGridType) {
            _y = y + elem_shift.y() + i*elem_size.height();
            if(keRowNumberAll == number)
                num_for_draw = glb().pGrid->getRows()-i;
            else if(keRowNumberOdd == number) {
                if((glb().pGrid->getRows()-i)%2)
                    num_for_draw = glb().pGrid->getRows()-i;
                else
                    num_for_draw = -1;
            } else if(keRowNumberEven == number) {
                if(!((glb().pGrid->getRows()-i)%2))
                    num_for_draw = glb().pGrid->getRows()-i;
                else
                    num_for_draw = -1;
            }
            DrawVRulerElement(num_for_draw, _x, _y, painter);
        } else if(keGridTypeShift == glb().tGridData.nGridType) {
            if(!(i%2)) {
                _y = y + i*elem_shift.y();
                DrawVRulerElement(glb().pGrid->getRows()-i, _x, _y, painter);
            }
        }
    }
}

void  TGridDraw::DrawVRulerElement(int ind, int x, int y, QPainter *painter) {
    QSize  elem_size = getElemSize();

    // если border то ставим цвет, иначе цвет как фон
    if(glb().pGrid->getBorder()) {
        painter->setPen(QPen(glb().tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    } else {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

    painter->drawRect(x, y, m_vruler_size.width(), elem_size.height());

    QString str;
    if(ind > 0) str = QString::number(ind);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->drawText(x+(m_vruler_size.width()-5*str.length())/2, y+(3*elem_size.height())/4, str);
}

void  TGridDraw::DrawHRuler(int x, int y, ERowNumber number, QPainter *painter) {
    int _x = 0;
    int _y = 0;

    _y += y;

    QSize  elem_size = getElemSize();
//    QPoint elem_shift = getElemShift();

    int num_for_draw = -1;

    for(int i = 0; i < glb().pGrid->getColumns(); i++) {
        if(keGridTypeNormal == glb().tGridData.nGridType) {
            _x = x + i*elem_size.width();
            if(keRowNumberAll == number)
                num_for_draw = glb().pGrid->getColumns()-i;
            else if(keRowNumberOdd == number) {
                if((glb().pGrid->getColumns()-i)%2)
                    num_for_draw = glb().pGrid->getColumns()-i;
                else
                    num_for_draw = -1;
            } else if(keRowNumberEven == number) {
                if(!((glb().pGrid->getColumns()-i)%2))
                    num_for_draw = glb().pGrid->getColumns()-i;
                else
                    num_for_draw = -1;
            }
            DrawHRulerElement(num_for_draw, _x, _y, painter);
        } else if(keGridTypeShift == glb().tGridData.nGridType) {
            _x = x + i*elem_size.width();
            DrawHRulerElement(glb().pGrid->getColumns()-i, _x, _y, painter);
        }
    }
}

void  TGridDraw::DrawHRulerElement(int ind, int x, int y, QPainter *painter) {
    QSize  elem_size = getElemSize();

    // если border то ставим цвет, иначе цвет как фон
    if(glb().pGrid->getBorder()) {
        painter->setPen(QPen(glb().tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    } else {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

    painter->drawRect(x, y, elem_size.width(), m_hruler_size.height());

    QString str;
    if(ind > 0) str = QString::number(ind);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->drawText(x+(elem_size.width()-5*str.length())/2, y+(3*m_vruler_size.height())/4, str);
}

void  TGridDraw::DrawElements(int x, int y, QPainter *painter) {
    int _x = 0;
    int _y = 0;

    QSize  elem_size = getElemSize();
    QPoint elem_shift = getElemShift();

    for(int i = 0; i < glb().pGrid->getRows(); i++) {
        for(int j = 0; j < glb().pGrid->getColumns(); j++) {
            if(keGridTypeNormal == glb().tGridData.nGridType) {
                _x = x + elem_shift.x() + j*elem_size.width();
                _y = y + elem_shift.y() + i*elem_size.height();
                DrawElement(i, j, _x, _y, painter);
            } else if(keGridTypeShift == glb().tGridData.nGridType) {
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
    if(glb().pGrid->getBorder()) {
        painter->setPen(QPen(glb().tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    } else {
        if(glb().pGrid->getElement(i,j).getFill())
            painter->setPen(QPen(glb().tItemColor, 1, Qt::SolidLine, Qt::FlatCap));
        else
            painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    if(glb().pGrid->getElement(i,j).getFill())
        painter->setBrush(QBrush(glb().tItemColor, Qt::SolidPattern));
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

    if(keItemTypeRectan == glb().tGridData.nItemType) {
        size.setHeight(20);
        size.setWidth(10);
    } else if(keItemTypeSquare == glb().tGridData.nItemType) {
        size.setHeight(10);
        size.setWidth(10);
    }

    return size;
}

QPoint  TGridDraw::getElemShift() {
    QPoint shift(0, 0);

    if(keGridTypeNormal == glb().tGridData.nGridType) {
        shift.setX(0);
        shift.setY(0);
    } else if(keGridTypeShift == glb().tGridData.nGridType) {
        shift.setX(0);
        shift.setY(getElemSize().height()/2);
    }

    return shift;
}

//------------------------------------------------------------------------------
