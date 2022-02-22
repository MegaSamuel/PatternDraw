#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();

    QSize new_size;
    new_size.setWidth(rect.size().width()/2);
    new_size.setHeight(2*rect.size().height()/3);
    win.resize( new_size );

    int x = (rect.width() - win.width())/2;
    int y = (rect.height() - win.height())/2;

    // распологаем форму по центру основного экрана
    win.move(x, y);

    win.show();

    return app.exec();
}

//------------------------------------------------------------------------------
