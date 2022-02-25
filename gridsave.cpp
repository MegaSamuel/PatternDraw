#include "gridsave.h"

//------------------------------------------------------------------------------

TGridSave::TGridSave()
{

}

//------------------------------------------------------------------------------

void convToColor(t_color *color, const QColor& qcolor) {
    color->red = static_cast<uint8_t>(qcolor.red());
    color->green = static_cast<uint8_t>(qcolor.green());
    color->blue = static_cast<uint8_t>(qcolor.blue());
}

void convFromColor(const t_color& color, QColor& qcolor) {
    qcolor.setRgb(static_cast<int>(color.red), static_cast<int>(color.green), static_cast<int>(color.blue));
}

QColor convFromColor(const t_color& color) {
    return QColor(static_cast<int>(color.red), static_cast<int>(color.green), static_cast<int>(color.blue));
}

//------------------------------------------------------------------------------
