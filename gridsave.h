#ifndef TGRIDSAVE_H
#define TGRIDSAVE_H

#include <QObject>
#include <QColor>

//------------------------------------------------------------------------------

class TGridSave : QObject
{
    Q_OBJECT

public:
    TGridSave();
};

#pragma pack(push, 1)

struct t_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct t_cell {
    uint8_t count;
    t_color color;
};

struct t_grid_data {
    uint8_t   id[3];

    uint8_t   item_type;
    uint8_t   grid_type;

    uint8_t   row;
    uint8_t   column;

    uint8_t   is_filled;

    t_color   t_grid_color;
    t_color   t_back_color;

    t_color   *grid;
};

#pragma pack(pop)

void convToColor(t_color *color, const QColor& qcolor);

void convFromColor(const t_color& color, QColor& qcolor);
QColor convFromColor(const t_color& color);

#endif // TGRIDSAVE_H

//------------------------------------------------------------------------------
