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

    m_need_to_emit = true;

    m_left_top_point = {0,0};
    m_right_bottom_point = {0,0};

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

QImage*  TGridDraw::getImage() {
    drawPicture();
    return &m_image;
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
        if(keGridTypeShift == glb().tGridData.nGridType) {
            if(isOdd(glb().pGrid->getRows())) y -= elem_size.height()/2;
        }
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
        y_shift += glb().pGrid->getRows()*elem_size.height() + glb().pGrid->getRows()-1;
    } else {
        y_shift += glb().pGrid->getRows()*elem_size.height()/2 + elem_size.height()/2 + glb().pGrid->getRows()/2-1;
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
    x_shift += glb().pGrid->getColumns()*elem_size.width() + glb().pGrid->getColumns()-1;

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

//    qDebug() << "pic" << m_pic_size.width() << m_pic_size.height();
//    qDebug() << "wgt" << this->width() << this->height();

    // если картинка стала больше видимой области по высоте
    if(m_pic_size.height() > this->height())
        this->setMinimumHeight(m_pic_size.height());

    // если картинка стала больше видимой области по ширине
    if(m_pic_size.width() > this->width())
        this->setMinimumWidth(m_pic_size.width());
}

void  TGridDraw::DrawVRuler(int x, int y, ERowNumber number, QPainter *painter) {
    int _x = x;
    int _y = y;

    QSize  elem_size = getElemSize();

    int num_for_draw = -1;

    for(int i = 0; i < glb().pGrid->getRows(); i++) {
        if(keGridTypeNormal == glb().tGridData.nGridType) {
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
            _y += elem_size.height()+1;
        } else if(keGridTypeShift == glb().tGridData.nGridType) {
            if(isEven(i)) {
                if(keRowNumberAll == number) {
                    if(isEven(glb().pGrid->getRows())) {
                        num_for_draw = glb().pGrid->getRows()-i-1;
                    } else {
                        num_for_draw = glb().pGrid->getRows()-i;
                    }
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
                _y += elem_size.height()+1;
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
    painter->setFont(QFont("MS Shell Dlg 2", 7));
    int shift_x = 0;
    int shift_y = 0;
    if(keItemTypeSquare == glb().tGridData.nItemType) {
        shift_x = (m_vruler_size.width()-5*str.length())/2;
        shift_y = (3*elem_size.height())/4;
    } else {
        shift_x = (m_vruler_size.width()-5*str.length())/2;
        shift_y = (5*elem_size.height())/8;
    }
    painter->drawText(x+shift_x, y+shift_y, str);
}

void  TGridDraw::DrawHRuler(int x, int y, ERowNumber number, QPainter *painter) {
    int _x = x;
    int _y = y;

    Q_UNUSED(number)

    QSize  elem_size = getElemSize();

    for(int i = 0; i < glb().pGrid->getColumns(); i++) {
        DrawHRulerElement(i+1, _x, _y, painter);
        _x += elem_size.width()+1;
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
    painter->setFont(QFont("MS Shell Dlg 2", 7));
    painter->drawText(x+(m_hruler_size.width()-5*str.length())/2, y+(3*m_hruler_size.height())/4, str);
}

void  TGridDraw::DrawElements(int x, int y, QPainter *painter) {
    int _x = x;
    int _y = y;

    QSize  elem_size = getElemSize();
    QPoint elem_shift = getElemShift();

    int shift = 0;

    for(int i = 0; i < glb().pGrid->getRows(); i++) {
        for(int j = 0; j < glb().pGrid->getColumns(); j++) {
            if(keGridTypeNormal == glb().tGridData.nGridType) {
                DrawElement(i, j, _x, _y, painter);
                _x += elem_size.width()+1;
            } else if(keGridTypeShift == glb().tGridData.nGridType) {
                if(isEven(i)) {
                    if(isEven(j)) {
                        // четная строка, четный столбец
                        _x = x + j*elem_size.width() + j;
                        _y = y + i*elem_shift.y() + shift;
                        DrawElement(i, j, _x, _y, painter);
                    }
                } else {
                    if(isOdd(j)) {
                        // нечетная строка, нечетный столбец
                        _x = x + j*elem_size.width() + j;
                        _y = y + i*elem_shift.y() + shift - 1;
                        DrawElement(i, j, _x, _y, painter);
                    }
                }
            }
        }

        if(keGridTypeNormal == glb().tGridData.nGridType) {
            _x = x;
            _y += elem_size.height()+1;
        } else if(keGridTypeShift == glb().tGridData.nGridType) {
            if(isEven(i)) shift += 1;
        }
    }
}

void  TGridDraw::DrawElement(int i, int j, int x, int y, QPainter *painter) {
    QSize  elem_size = getElemSize();

    if(!glb().pGrid->getSplit()) {
        // если border то ставим цвет, иначе цвет как фон
        if(glb().pGrid->getBorder()) {
            painter->setPen(QPen(glb().tGridColor, 1, Qt::SolidLine, Qt::FlatCap));
        } else {
            if(glb().pGrid->getElement(i,j).getFill())
                painter->setPen(QPen(glb().pGrid->getColor(i, j), 1, Qt::SolidLine, Qt::FlatCap));
            else if(glb().pGrid->getElement(i,j).getBackFill())
                painter->setPen(QPen(glb().pGrid->getBackColor(i, j), 1, Qt::SolidLine, Qt::FlatCap));
            else
                painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
        }
    }

    if((keGridTypeShift == glb().tGridData.nGridType) && glb().pGrid->getSplit()) {
        // белая часть
        painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::FlatCap));
        painter->drawRect(x, y, elem_size.width(), elem_size.height()/2);

        // цветная часть
        if(glb().pGrid->getElement(i,j).getFill()) {
            painter->setBrush(QBrush(glb().pGrid->getColor(i, j), Qt::SolidPattern));
            painter->setPen(QPen(glb().pGrid->getColor(i, j), 1, Qt::SolidLine, Qt::FlatCap));
        } else {
            painter->setBrush(QBrush(glb().pGrid->getBackColor(i, j), Qt::SolidPattern));
            painter->setPen(QPen(glb().pGrid->getBackColor(i, j), 1, Qt::SolidLine, Qt::FlatCap));
        }

        painter->drawRect(x, y+elem_size.height()/2, elem_size.width(), elem_size.height()/2);
    } else {
        if(glb().pGrid->getElement(i,j).getFill())
            painter->setBrush(QBrush(glb().pGrid->getColor(i, j), Qt::SolidPattern));
        else if(glb().pGrid->getElement(i,j).getBackFill())
            painter->setBrush(QBrush(glb().pGrid->getBackColor(i, j), Qt::SolidPattern));
        else
            painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));

        painter->drawRect(x, y, elem_size.width(), elem_size.height());
    }
}

//------------------------------------------------------------------------------

void  TGridDraw::onChangeGridColor(QColor color) {
    Q_UNUSED(color)
    //qDebug() << "change grid color";
    update();
}

void  TGridDraw::onChangeBackColor(QColor color) {
    glb().pGrid->setBackColor(color);
    //qDebug() << "change back color";
    update();
}

//------------------------------------------------------------------------------

void  TGridDraw::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        // только если курсор на таблице
        if((0 == m_curr_row) || (0 == m_curr_column))
            return;

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
        // только если курсор на таблице
        if((0 == m_curr_row) || (0 == m_curr_column))
            return;

        // по правой кнопке ничего не делаем
    }
}

void  TGridDraw::mouseMoveEvent(QMouseEvent *event) {
    Q_UNUSED(event)

    int column = calcColumnNum(event->pos().x());
    int y_shift_top = 0;
    int y_shift_bottom = 0;
    QPoint elem_shift = getElemShift();

    // если четный столбец (отсчет слева)
    if(isEven(glb().pGrid->getColumns()-column+1)) {
        if(isEven(glb().pGrid->getRows())) {
            // четное количество рядов
            y_shift_top = elem_shift.y();
        } else {
            // нечетное количество рядов
            y_shift_top = elem_shift.y();
            y_shift_bottom = elem_shift.y();
        }
    } else {
        if(isEven(glb().pGrid->getRows())) {
            // четное количество рядов
            y_shift_bottom = elem_shift.y();
        }
    }

    // если курсор вне таблицы
    if((event->pos().x() < m_left_top_point.x()) || (event->pos().y() < (m_left_top_point.y()+y_shift_top)) ||
       (event->pos().x() > m_right_bottom_point.x()) || (event->pos().y() > (m_right_bottom_point.y()-y_shift_bottom))) {
        if(m_need_to_emit) {
            m_need_to_emit = false;

            m_curr_row = 0;
            m_curr_column = 0;

            m_prev_row = 0;
            m_prev_column = 0;

            Q_EMIT(currentPos(-1, -1));
        }
        return;
    }

    m_need_to_emit = true;

    // расчет положения
    // сначала считаем петли - они не зависят от смещения рядов
    //m_curr_column = calcColumnNum(event->pos().x());
    m_curr_column = column;
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
        Q_EMIT(currentPos(m_curr_row, (glb().pGrid->getColumns()-m_curr_column+1)));

        //qDebug() << "row" << m_curr_row << "column" << (glb().pGrid->getColumns()-m_curr_column+1);
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
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/(elem_size.height()+1);

                if(isEven(glb().pGrid->getRows())) {
                    if(isOdd(ind)) ind += 1;
                } else {
                    if(isEven(ind)) ind += 1;
                }
            } else {
                // находимся на четной петле
                shift = elem_size.height()/2;
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/(elem_size.height()+1);

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
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/(elem_size.height()+1);

                if(isEven(glb().pGrid->getRows())) {
                    if(isEven(ind)) ind -= 1;
                } else {
                    if(isOdd(ind)) ind -= 1;
                }
            } else {
                // находимся на четной петле
                ind = glb().pGrid->getRows() - 2*(y - m_left_top_point.y() - shift)/(elem_size.height()+1);

                if(isEven(glb().pGrid->getRows())) {
                    if(isOdd(ind)) ind += 1;
                } else {
                    if(isEven(ind)) ind += 1;
                }
            }
        }
    } else {
        ind = glb().pGrid->getRows() - (y - m_left_top_point.y())/(elem_size.height()+1);
    }

    if(ind < 1) ind = 1;
    if(ind > glb().pGrid->getRows()) ind = glb().pGrid->getRows();

    return ind;
}

int  TGridDraw::calcColumnNum(int x) {
    assert(0 != m_right_bottom_point.x());

    QSize  elem_size = getElemSize();

    int ind = glb().pGrid->getColumns() - (x - m_left_top_point.x())/(elem_size.width()+1);

//    qDebug() << "col 1" << glb().pGrid->getColumns() << ind;
//    qDebug() << "col 2" << x << m_left_top_point.x();

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
