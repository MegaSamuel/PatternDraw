#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect size = screen->availableGeometry();

    int x = (size.width() - win.width()) / 2;
    int y = (size.height() - win.height()) / 2;

    // распологаем форму по центру основного экрана
    win.move(x, y);

    win.show();

    return app.exec();
}

//------------------------------------------------------------------------------
