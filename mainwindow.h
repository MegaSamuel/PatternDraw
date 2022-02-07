#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <bitmap.h>
#include <QDir>
#include <QFile>
#include <QMainWindow>

//------------------------------------------------------------------------------

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
    void       onBtnSave(); //obsolete
    void       onBtnPreview();
    void       onBtnChangeBackColor();
    void       onBtnChangeGridColor();
    void       onNewHandler();
    void       onOpenHandler();
    void       onSaveHandler();
    void       onSaveAsHandler();
    void       onPrintHandler();
    void       onQuitHandler();
    void       onUndoHandler();
    void       onRedoHandler();
    void       onInfoHandler();
    void       onManHandler();
    void       onChangeItem(int  index);
    void       onChangeSize(int  index);
    void       onChangeGrid(int  index);

private:
    Ui::MainWindow *ui;

    bool           askSaveIfChanged(const QString& discard = "");

    QString        m_zPrgName;
    QString        m_zPrgTitle;
    bool           m_bPrgTitleChanged;

    QString        m_zPrgFileName;

    void           setPrgTitleText( const QString&  text = "" );

    void           setPrgTitleChanged( bool  changed );

    bool           imageCreate();

    bool           imageFillNormal();
    bool           imageFillShift();

    void           setCellSize();
    void           getBackColor( RGBQUAD  *a_pColor );
    void           getGridColor( RGBQUAD  *a_pColor );

    void           setLabelBackColor( QLabel  *a_pLabel, QColor  *a_pColor );

    bool           fileSave();
    bool           fileSaveAs();

    unsigned       m_uItemType;
    unsigned       m_uItemSize;
    unsigned       m_uGridType;

    TBitMap        m_tBitMap;

    TCell          m_tCell;

    unsigned       m_uRow;
    unsigned       m_uColumn;

    QPixmap       *m_pPixmap;
    QByteArray    *m_pImage;

    bool           m_bImageReady;

    QColor         m_tBackColor;
    QColor         m_tGridColor;

    QColorDialog   m_tColorDialog;

    void           closeEvent( QCloseEvent *event );
    void           resizeEvent( QResizeEvent *event );
};

//------------------------------------------------------------------------------

#endif // MAINWINDOW_H
