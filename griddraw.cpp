#include <QDebug>

#include <cassert>

#include "griddraw.h"
#include "global.h"

//------------------------------------------------------------------------------

TGridDraw::TGridDraw(QWidget *parent) : QWidget(parent) {
    m_curr_row = 0;
    m_curr_column = 0;

    m_prev_row = 0;
    m_prev_column = 0;

    m_hruler_size.setHeight(SHORT_SIDE);
    m_hruler_size.setWidth(SHORT_SIDE);

    m_vruler_size.setHeight(SHORT_SIDE);
    m_vruler_size.setWidth(SHORT_SIDE);

    // включаем событие mouse move без нажатия кнопок
    this->setMouseTracking(true);
}

TGridDraw::~TGridDraw() {

}

//------------------------------------------------------------------------------

void  TGridDraw::updateRulerSize() {
    QSize  elem_size = getElemSize();

    m_hruler_size.setHeight(SHORT_SIDE);
    m_hruler_size.setWidth(elem_size.width());

    m_vruler_size.setHeight(elem_size.height());
    m_vruler_size.setWidth(SHORT_SIDE);
}

//------------------------------------------------------------------------------

/* Метод, в котором происходит рисование
 * */
void  TGridDraw::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this); // Создаём объект отрисовщика

    // рисуем на форму (qwidget)
    drawAll(&painter);
}

bool  TGridDraw::saveImage(const QString &fileName, const char *format) {
    drawPicture();
    return m_image.save(fileName, format);
}

void  TGridDraw::drawPicture() {
    assert(0 != m_pic_size.width());
    assert(0 != m_pic_size.height());

    // m_pic_size заполнится при рисовании на виджете
    QImage image(m_pic_size, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image); // Создаём объект отрисовщика

    // рисуем в qimage
    drawAll(&painter);

    m_image = image;
}

void  TGridDraw::drawAll(QPainter *painter) {
    int x_shift = 0;
    int y_shift = 0;

    int y_shift_start = 0;

    updateRulerSize();

    QSize  elem_size = getElemSize();

    // рулетка слева
    if(glb().pGrid->getRulerV() && (keRulerTypeRightLeft == glb().pGrid->getRulerVtype())) {
        int y = y_shift;
        if(isOdd(glb().pGrid->getRows())) y -= elem_size.height()/2;
        DrawVRuler(x_shift, y, keRowNumberEven, painter);
        // сдвигаемся на ширину вертикальной рулетки
        x_shift += m_vruler_size.width();
        x_shift += 1; //add one
    }

    // собственно табличка ячеек
    DrawElements(x_shift, y_shift, painter);

    // запоминаем левый верхний угол таблицы
    m_left_top_point.setX(x_shift);
    m_left_top_point.setY(y_shift);

    // сдвигаемся на высоту таблицы
    if(keGridTypeNormal == glb().tGridData.nGridType) {
        y_shift += glb().pGrid->getRows()*elem_size.height();
    } else {
        y_shift += glb().pGrid->getRows()*elem_size.height()/2 + elem_size.height()/2;
    }

    // запоминаем правый нижний угол таблицы
    m_right_bottom_point.setY(y_shift);

    y_shift += 1; //add one

    // рулетка снизу
    if(glb().pGrid->getRulerH()) {
        DrawHRuler(x_shift, y_shift, keRowNumberAll, painter);
        y_shift += m_hruler_size.height();
    }

    // сдвигаемся на ширину таблицы
    x_shift += glb().pGrid->getColumns()*elem_size.width();

    // запоминаем правый нижний угол таблицы
    m_right_bottom_point.setX(x_shift);

    x_shift += 1; //add one

    if((keGridTypeShift == glb().tGridData.nGridType) && isEven(glb().pGrid->getRows())) {
        y_shift_start += elem_size.height()/2;
    }

    // рулетка справа
    if(glb().pGrid->getRulerV()) {
        if(keRulerTypeRightLeft == glb().pGrid->getRulerVtype())
            DrawVRuler(x_shift, y_shift_start, keRowNumberOdd, painter);
        else
            DrawVRuler(x_shift, y_shift_start, keRowNumberAll, painter);
        // сдвигаемся на ширину вертикальной рулетки
        x_shift += m_vruler_size.width();
        //x_shift += 1; //add one
    }

    // плюс 1 т.к. координаты идут с нуля
    m_pic_size.setWidth(x_shift+1);
    m_pic_size.setHeight(y_shift+1);
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
            if(isEven(i)) {
                _y = y + i*elem_shift.y();
                if(keRowNumberAll == number) {
                    num_for_draw = glb().pGrid->getRows()-i-1;
                } else if(keRowNumberOdd == number) {
                    if(isEven(glb().pGrid->getRows())) {
                        num_for_draw = glb().pGrid->getRows()-i-1;
                    } else {
                        num_for_draw = glb().pGrid->getRows()-i;
                    }
                } else if(keRowNumberEven == number) {
                    if(isEven(glb().pGrid->getRows())) {
                        num_for_draw = glb().pGrid->getRows()-i;
                    } else {
                        num_for_draw = glb().pGrid->getRows()-i+1;
                    }
                }

                // рисуем только если влезаем в widget
                if((0 <= _x) && (0 <= _y)) {
                    DrawVRulerElement(num_for_draw, _x, _y, painter);
                }
            }
        }
    }
}

void  TGridDraw::DrawVRulerElement(int ind, int x, int y, QPainter *painter) {
    QSize  elem_size = getElemSize();

    // если border то ставим цвет, иначе цвет как фон
    if(glb().pGrid->getRulerBorder()) {
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
    if(glb().pGrid->getRulerBorder()) {
        painter->setPen(QPen(glb().tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
    } else {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

    painter->drawRect(x, y, elem_size.width(), m_hruler_size.height());

    QString str;
    if(ind > 0) str = QString::number(ind);
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter->drawText(x+(m_hruler_size.width()-5*str.length())/2, y+(3*m_hruler_size.height())/4, str);
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
            painter->setPen(QPen(glb().pGrid->getColor(i, j), 1, Qt::SolidLine, Qt::FlatCap));
        else
            painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
    }

    if(glb().pGrid->getElement(i,j).getFill())
        painter->setBrush(QBrush(glb().pGrid->getColor(i, j), Qt::SolidPattern));
    else
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

    painter->drawRect(x, y, elem_size.width(), elem_size.height());
}

void  TGridDraw::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        // по левой кнопке красим ячейку
        int row = glb().pGrid->getRows() - m_curr_row;
        int col = glb().pGrid->getColumns() - m_curr_column;

        glb().pGrid->setColor(row, col, glb().tItemColor);

//        qDebug() << "rows" << glb().pGrid->getRows() << "colomns" << glb().pGrid->getColumns();
//        qDebug() << "c_row" << m_curr_row << "c_colomn" << m_curr_column;
//        qDebug() << "ind_row" << row << "int_colomn" << col;

        Q_EMIT(changeState());

        repaint();
    } else if(event->button() == Qt::RightButton) {
        // по правой кнопке ничего не делаем
    }
}

void  TGridDraw::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)

    // если курсор вне картинки
    if((event->pos().x()+1 > m_pic_size.width()) || (event->pos().y()+1 > m_pic_size.height())) {
        return;
    }

    // расчет положения
    // сначала считаем петли - они не зависят от смещения рядов
    m_curr_column = calcColumnNum(event->pos().x());
    m_curr_row = calcRowNum(event->pos().y());

    bool need_to_emit = false;

    if(m_curr_row != m_prev_row) {
        m_prev_row = m_curr_row;
        need_to_emit = true;
    }

    if(m_curr_column != m_prev_column) {
        m_prev_column = m_curr_column;
        need_to_emit = true;
    }

    // если положение изменилось - отправляем сигнал
    if(need_to_emit) {
        Q_EMIT(currentPos(m_curr_row, m_curr_column));

//        qDebug() << "row" << m_curr_row << "column" << m_curr_column;
    }

//    if(event->button() == Qt::LeftButton) {
//        qDebug() << "move with left; row" << m_curr_row << "column" << m_curr_column;
//    }

//    if(event->button() == Qt::RightButton) {
//        qDebug() << "move with left; row" << m_curr_row << "column" << m_curr_column;
//    }
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

int TGridDraw::getCurrRow() const {
    return m_curr_row;
}

int TGridDraw::getCurrColumn() const {
    return m_curr_column;
}

int  TGridDraw::calcRowNum(int y) {
    assert(0 != m_right_bottom_point.y());

    QSize  elem_size = getElemSize();

    int ind = 0;
    int shift  = 0;

    if(keGridTypeShift == glb().tGridData.nGridType) {
        if(isOdd(glb().pGrid->getColumns())) {
            // нечетное кол-во петель
            if(isOdd(getCurrColumn())) {
                // находимся на нечетной петле
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/elem_size.height();

                if(isEven(glb().pGrid->getRows())) {
                    if(isOdd(ind)) ind += 1;
                } else {
                    if(isEven(ind)) ind += 1;
                }
            } else {
                // находимся на четной петле
                shift = elem_size.height()/2;
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/elem_size.height();

                if(isEven(glb().pGrid->getRows())) {
                    if(isEven(ind)) ind -= 1;
                } else {
                    if(isOdd(ind)) ind -= 1;
                }
            }
        } else {
            // четное кол-во петель
            if(isOdd(getCurrColumn())) {
                // находимся на нечетной петле
                shift = elem_size.height()/2;
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/elem_size.height();

                if(isEven(glb().pGrid->getRows())) {
                    if(isEven(ind)) ind -= 1;
                } else {
                    if(isOdd(ind)) ind -= 1;
                }
            } else {
                // находимся на четной петле
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/elem_size.height();

                if(isEven(glb().pGrid->getRows())) {
                    if(isOdd(ind)) ind += 1;
                } else {
                    if(isEven(ind)) ind += 1;
                }
            }
        }
    } else {
        ind = glb().pGrid->getRows() - (y - m_left_top_point.y())/elem_size.height();
    }

    if(ind < 1) ind = 1;
    if(ind > glb().pGrid->getRows()) ind = glb().pGrid->getRows();

    return ind;
}

int  TGridDraw::calcColumnNum(int x) {
    assert(0 != m_right_bottom_point.x());

    QSize  elem_size = getElemSize();

    int ind = glb().pGrid->getColumns() - (x - m_left_top_point.x())/elem_size.width();

    if(ind < 1) ind = 1;
    if(ind > glb().pGrid->getColumns()) ind = glb().pGrid->getColumns();

    return ind;
}

QSize  TGridDraw::getElemSize() {
    QSize size(SHORT_SIDE, SHORT_SIDE);

    if(keItemTypeRectan == glb().tGridData.nItemType) {
        size.setHeight(LONG_SIDE);
        size.setWidth(SHORT_SIDE);
    } else if(keItemTypeSquare == glb().tGridData.nItemType) {
        size.setHeight(SHORT_SIDE);
        size.setWidth(SHORT_SIDE);
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
