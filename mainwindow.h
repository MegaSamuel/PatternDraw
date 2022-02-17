#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QFile>
#include <QMainWindow>

#include <bitmap.h>

#include "global.h"
#include "newdialog.h"

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

private Q_SLOTS:
    void       onCurrentPos(int, int);
    void       onChangeState();
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
    void       onChangeGrid(int  index);

    void on_spinRow_valueChanged(int arg1);
    void on_spinColumn_valueChanged(int arg1);

    void on_btnRowM_clicked();
    void on_btnRowP_clicked();
    void on_btnColumnM_clicked();
    void on_btnColumnP_clicked();

    void on_checkBoxGrid_stateChanged(int arg1);
    void on_checkBoxGridRuler_stateChanged(int arg1);
    void on_checkBoxSplit_stateChanged(int arg1);

    void       onDlgCreate();

    void on_checkBoxRulerV_stateChanged(int arg1);
    void on_checkBoxRulerH_stateChanged(int arg1);

    void on_radioRulerV1_clicked();
    void on_radioRulerV2_clicked();

    void on_radioRulerH1_clicked();
    void on_radioRulerH2_clicked();

private:
    Ui::MainWindow *ui;

    void           initGuiElements(bool first_start = false);

    bool           askSaveIfChanged(const QString& discard = "");

    QString        m_zPrgName;
    QString        m_zPrgTitle;
    bool           m_bPrgTitleChanged;

    QString        m_zPrgFileName;

    void           setPrgTitleText( const QString&  text = "" );

    void           setPrgTitleChanged( bool  changed );

    void           getBackColor( RGBQUAD  *a_pColor );
    void           getGridColor( RGBQUAD  *a_pColor );

    void           setLabelBackColor( QLabel  *a_pLabel, QColor  *a_pColor );

    bool           fileSave();
    bool           fileSaveAs();

    bool           fileSaveToDev(const QString& filename);

    TNewDialog    *m_ptNewDialog;

    unsigned       m_uItemType;
    unsigned       m_uGridType;

    TBitMap        m_tBitMap;

    unsigned       m_uRow;
    unsigned       m_uColumn;

    unsigned       m_uCurrRow;
    unsigned       m_uCurrColumn;

    bool           m_bImageReady; //TODO перевесить признак на картинку QPainter

    void           guiBlock(bool block);

    QColor         m_tBackColor;
    QColor         m_tGridColor;

    QColorDialog   m_tColorDialog;

    void           closeEvent( QCloseEvent *event );
    void           resizeEvent( QResizeEvent *event );

    void           setStateChanged();
    void           resetStateChanged();

    void           showInfoMessage(const QString& msg);

    TGrid         *m_pGrid;
};

//------------------------------------------------------------------------------

#endif // MAINWINDOW_H
