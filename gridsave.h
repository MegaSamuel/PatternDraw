#ifndef TGRIDSAVE_H
#define TGRIDSAVE_H

#include <QObject>

//------------------------------------------------------------------------------

#pragma pack(push, 1)

struct t_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct t_grid_data {
    char      id1;
    char      id2;

    uint8_t   item_type;
    uint8_t   grid_type;

    uint8_t   row;
    uint8_t   column;

    bool      is_filled;

    t_color   t_grid_color;
    t_color   t_back_color;

    t_color   *grid;
};

#pragma pack(pop)

class TGridSave : QObject
{
    Q_OBJECT

public:
    TGridSave();
};

#endif // TGRIDSAVE_H

//------------------------------------------------------------------------------
