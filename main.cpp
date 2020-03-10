#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // распологаем форму по центру основного экрана
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);

    // задаем фиксированный размер окна
    w.setFixedSize( 320, 240 );

    // определяем шрифт
    QFont newFont( "Tahoma", 8, QFont::Thin, false );
    // применяем ко всему приложению
    QApplication::setFont(newFont);

    w.show();

    return a.exec();
}
