#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <bitmap.h>
#include <QDir>
#include <QFile>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = Q_NULLPTR );
    ~MainWindow();

    enum EGridType
    {
        keGridTypeNormal = 0,
        keGridTypeShift  = 1,
    };

    enum EItemType
    {
        keItemTypeRectan = 0,
        keItemTypeSquare = 1,
    };

    enum EItemSize
    {
        keItemSizeSmall  = 0,
        keItemSizeNormal = 1,
        keItemSizeHuge   = 2,
    };

private Q_SLOTS:
    void       onButtonHandler();
    void       onInfoHandler();
    void       onManHandler();
    void       onChangeItem( int  index );
    void       onChangeSize( int  index );
    void       onChangeGrid( int  index );

private:
    Ui::MainWindow *ui;

    void       BitMapCreate( QFile  *file ); //obsolete
    void       BitMapFill( QFile  *file );   //obsolete

    void       fileCreate( QFile  *file );

    void       fileFillNormal( QFile  *file );
    void       fileFillShift( QFile  *file );

    void       setCellSize();

    unsigned   m_uItemType;
    unsigned   m_uItemSize;
    unsigned   m_uGridType;

    TBitMap    m_tBitMap;

    TCell      m_tCell;

    unsigned   m_uRow;
    unsigned   m_uColumn;
};

#endif // MAINWINDOW_H
