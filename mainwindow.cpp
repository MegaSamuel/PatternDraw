#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

#include <cassert>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "griddraw.h"
#include "global.h"

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_bPrgTitleChanged = false;

    m_zPrgName = "PatternDraw";

    initGuiElements(true);

    // диалог Новыя сетка
    m_ptNewDialog = new TNewDialog(this);
    connect(m_ptNewDialog, &TNewDialog::dlgCreate, this, &MainWindow::onDlgCreate);

    // таблица элементов
    assert(0 != m_uRow);
    assert(0 != m_uColumn);
    m_pGrid = new TGrid(static_cast<int>(m_uRow), static_cast<int>(m_uColumn));

    // ловим сигнал с состоянии undo
    connect(m_pGrid, &TGrid::undoFilled, this, &MainWindow::onUndoFilled);
    // ловим сигнал с состоянии redo
    connect(m_pGrid, &TGrid::redoFilled, this, &MainWindow::onRedoFilled);

    ui->tGridDraw->setVisible(false);

    // ловим сигнал с номером текущей ячейки
    connect(ui->tGridDraw, &TGridDraw::currentPos, this, &MainWindow::onCurrentPos);
    // ловим сигнал об изменении картинки
    connect(ui->tGridDraw, &TGridDraw::changeState, this, &MainWindow::onChangeState);

    // отправляем указатель на таблицу в рисовалку
    assert(nullptr != m_pGrid);
    glb().pGrid = m_pGrid;

    // сформировано ли изображение
    m_bImageReady = false;

    guiBlock(true);

    // ставим стиль
    QApplication::setStyle(QStyleFactory::create("fusion"));

    // заголовок формы
    setPrgTitleText();

    // иконка формы
    setWindowIcon( QIcon( ":/PatternDraw.ico" ) );

    // ловим нажатие кнопки Изменить цвет фона
    connect( ui->btnBackColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeBackColor ) ;

    // ловим нажатие кнопки Изменить цвет сетки
    connect( ui->btnGridColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeGridColor ) ;

    // меню Файл
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewHandler);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenHandler);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveHandler);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsHandler);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::onPrintHandler);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuitHandler);

    // меню Правка
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndoHandler);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedoHandler);

    // вызов руководства пользователя
    connect(ui->actionMan, &QAction::triggered, this, &MainWindow::onManHandler);

    // вызов справки о программе
    connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::onInfoHandler);

    // комбобокс с типом элемента
    connect( ui->comboBoxItem, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onChangeItem );

    // комбобокс с типом строки
    connect( ui->comboBoxGrid, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onChangeGrid );

    // центральный элемент
    setCentralWidget( ui->centralWidget );

    // центруем лайбл с превью
    //ui->lblPicture->setAlignment( Qt::AlignCenter );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::initGuiElements(bool first_start) {
    m_uRow = static_cast<unsigned>(glb().tGridData.nRow);
    m_uColumn = static_cast<unsigned>(glb().tGridData.nColumn);

    m_uCurrRow = 1;
    m_uCurrColumn = 1;

    m_uItemType = static_cast<unsigned>(glb().tGridData.nItemType);
    m_uGridType = static_cast<unsigned>(glb().tGridData.nGridType);

    if(!first_start) {
        ui->checkBoxSplit->setEnabled(((keGridTypeShift == m_uGridType) && (keItemTypeRectan == m_uItemType)));
    }

    ui->spinRow->setValue(static_cast<int>(m_uRow));
    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    ui->spinCurrRow->setValue(static_cast<int>(m_uCurrRow));
    ui->spinCurrColumn->setValue(static_cast<int>(m_uCurrColumn));

    ui->comboBoxItem->setCurrentIndex(static_cast<int>(m_uItemType));
    ui->comboBoxGrid->setCurrentIndex(static_cast<int>(m_uGridType));

    // цвет элемента
    m_tBackColor = Qt::white;
    glb().tItemColor = Qt::white;
    setLabelBackColor(ui->labelBackColor, &m_tBackColor);

    // цвет сетки
    m_tGridColor = Qt::gray;
    glb().tGridColor = Qt::gray;
    setLabelBackColor(ui->labelGridColor, &m_tGridColor);

    ui->radioRulerH1->setChecked(true);
    ui->radioRulerV1->setChecked(true);
}

// изначально часть gui заблокировано
// снимаем блокировку после создания новой таблички (Ctrl+N)s
void  MainWindow::guiBlock(bool block) {
    ui->btnRowP->setEnabled(!block);
    ui->btnRowM->setEnabled(!block);
    ui->btnColumnP->setEnabled(!block);
    ui->btnColumnM->setEnabled(!block);
}

//------------------------------------------------------------------------------

void  MainWindow::getBackColor( RGBQUAD  *a_pColor )
{
    a_pColor->rgbRed = static_cast<BYTE>(m_tBackColor.red());
    a_pColor->rgbGreen = static_cast<BYTE>(m_tBackColor.green());
    a_pColor->rgbBlue = static_cast<BYTE>(m_tBackColor.blue());
    a_pColor->rgbReserved = 0x0;
}

void  MainWindow::getGridColor( RGBQUAD  *a_pColor )
{
    a_pColor->rgbRed = static_cast<BYTE>(m_tGridColor.red());
    a_pColor->rgbGreen = static_cast<BYTE>(m_tGridColor.green());
    a_pColor->rgbBlue = static_cast<BYTE>(m_tGridColor.blue());
    a_pColor->rgbReserved = 0x0;
}

void  MainWindow::setLabelBackColor( QLabel  *a_pLabel, QColor  *a_pColor )
{
    QString style = "QLabel {background-color: rgb(%1, %2, %3);}";

    a_pLabel->setStyleSheet( style.arg(a_pColor->red()).arg(a_pColor->green()).arg(a_pColor->blue()) );
}

//------------------------------------------------------------------------------

void  MainWindow::setPrgTitleText( const QString&  text )
{
    if( 0 != text.length() )
    {
        m_zPrgTitle = text + " - " + m_zPrgName;

        if( m_bPrgTitleChanged )
        {
            m_zPrgTitle.prepend( "*" );
        }
    }
    else
    {
        m_zPrgTitle = m_zPrgName;
    }

    setWindowTitle( m_zPrgTitle );
}

void  MainWindow::setPrgTitleChanged( bool  changed )
{
    if( changed )
    {
        m_zPrgTitle.prepend( "*" );

        setWindowTitle( m_zPrgTitle );
    }
    else
    {
        m_zPrgTitle.replace( "*", "" );

        setWindowTitle( m_zPrgTitle );
    }
}

//------------------------------------------------------------------------------

bool  MainWindow::fileSaveToDev(const QString& filename) {
    bool result;
    QString  format = filename.right(3).toUpper();

    result = ui->tGridDraw->saveImage(filename, format.toStdString().c_str());

    resetStateChanged();

    if(!result) {
        qDebug() << "Ошибка записи в файл";
    }

    return result;
}

bool  MainWindow::fileSave() {
    bool result = false;

    if(!m_zPrgFileName.isEmpty()) {
        result = fileSaveToDev(m_zPrgFileName);
    }

    return result;
}

bool  MainWindow::fileSaveAs() {
    bool result = false;

    // формируем имя файла по умолчанию
    QString deffilename = QString("/pattern%1x%2").arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir(QDir::currentPath() + deffilename);

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", dir, "PNG (*.png);;JPEG (*.jpg);;Bitmap picture (*.bmp)");

    QApplication::processEvents();

    if(!filename.isEmpty()) {
        m_zPrgFileName = filename;
        result = fileSaveToDev(m_zPrgFileName);
    } else {
        qDebug() << "no filename";
    }

    return result;
}

//------------------------------------------------------------------------------

void  MainWindow::setStateChanged() {
    if(!m_bPrgTitleChanged) {
        m_bPrgTitleChanged = true;
        setPrgTitleChanged(true);
    }
}

void  MainWindow::resetStateChanged() {
    if(m_bPrgTitleChanged) {
        setPrgTitleChanged(false);
        m_bPrgTitleChanged = false;
    }
}

void  MainWindow::onCurrentPos(int row, int col) {
    m_uCurrRow = static_cast<unsigned>(row);
    m_uCurrColumn = static_cast<unsigned>(col);

    ui->spinCurrRow->setValue(row);
    ui->spinCurrColumn->setValue(col);
}

void  MainWindow::onChangeState() {
    setStateChanged();
}

void  MainWindow::onBtnChangeBackColor()
{
    QColor color;

    //!bug почему-то не работает
    m_tColorDialog.setCurrentColor( m_tBackColor );

    color = m_tColorDialog.getColor();

    if( color.isValid() )
    {
        m_tBackColor = color;

        setLabelBackColor( ui->labelBackColor, &m_tBackColor );

        glb().tItemColor = color;
    }
}

void  MainWindow::onBtnChangeGridColor()
{
    QColor color;

    //!bug почему-то не работает
    m_tColorDialog.setCurrentColor( m_tGridColor );

    color = m_tColorDialog.getColor();

    if( color.isValid() )
    {
        m_tGridColor = color;

        setLabelBackColor( ui->labelGridColor, &m_tGridColor );

        glb().tGridColor = color;

        ui->tGridDraw->update();
    }
}

void  MainWindow::onNewHandler() {
    // есть несохраненные изменения
    if(!askSaveIfChanged()) {
        return;
    }

    m_ptNewDialog->open();
}

void  MainWindow::onDlgCreate() {
    m_zPrgTitle.clear();

    m_bPrgTitleChanged = false;

    m_zPrgFileName.clear();

    initGuiElements();

    glb().pGrid->initCells();

    ui->tGridDraw->setVisible(true);

    // сформировано ли изображение
    m_bImageReady = true;

    guiBlock(false);

    // заголовок формы
    setPrgTitleText();
}

void  MainWindow::onOpenHandler() {

}

void  MainWindow::onSaveHandler() {
    // нет картинки
    if(!m_bImageReady) {
        showInfoMessage("Нет изображения!", "Перед сохранением создайте новую сетку.");
        return;
    }

    if(m_zPrgFileName.isEmpty()) {
        fileSaveAs();
    }
    else {
        fileSave();
    }
}

void  MainWindow::onSaveAsHandler() {
    // нет картинки
    if(!m_bImageReady) {
        showInfoMessage("Нет изображения!", "Перед сохранением создайте новую сетку.");
        return;
    }

    fileSaveAs();
}

void  MainWindow::onPrintHandler() {
    // нет картинки
    if(!m_bImageReady) {
        showInfoMessage("Нет изображения!", "Перед печатью создайте новую сетку.");
        return;
    }

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Печать сетки"));

    // нажали Ок
    if(dialog->exec() != QDialog::Accepted)
        return;

    // есть картинка

    //!TODO сейчас картинка помещается в левый верхний угол растягивается на весь лист
    //!добавить настройки: центровка, поля, масштаб?..

    QPainter painter(&printer);
    const QImage *image = ui->tGridDraw->getImage();
    QRect rect = painter.viewport();
    QSize size = image->size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(image->rect());
    painter.drawImage(0, 0, *image);
}

void  MainWindow::onQuitHandler() {
    if(askSaveIfChanged()) {
        QApplication::quit();
    }
}

void  MainWindow::onUndoHandler() {
    // отменяем действие
    if(m_pGrid->doUndo()) {
        ui->tGridDraw->update();
    }
}

void  MainWindow::onRedoHandler() {
    // отменяем отмену действия
    if(m_pGrid->doRedo()) {
        ui->tGridDraw->update();
    }
}

void  MainWindow::onInfoHandler()
{
    QString  zTitle;
    Info *pInfo = new Info;

    zTitle = "О программе " + m_zPrgName;

    pInfo->setWindowTitle( zTitle );
    pInfo->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );
    pInfo->setWindowFlags( Qt::WindowSystemMenuHint );
    //pInfo->setWindowFlag(Qt::WindowSystemMenuHint,false);

    pInfo->setMinimumWidth(500);
    pInfo->setMinimumHeight(200);

    pInfo->exec();
}

void  MainWindow::onManHandler()
{
    QString  zTitle;
    Man *pMan = new Man;

    zTitle = "Руководство " + m_zPrgName;

    pMan->setWindowTitle( zTitle );
    pMan->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );
    pMan->setWindowFlags( Qt::WindowSystemMenuHint );

    pMan->setMinimumWidth(500);
    pMan->setMinimumHeight(200);

    pMan->exec();
}

void  MainWindow::onChangeItem( int  index )
{
#if 0
    if( 0 == index )
        m_uItemType = keItemTypeRectan;
    else if( 1 == index )
        m_uItemType = keItemTypeSquare;

    glb().m_uItemType = m_uItemType;

    ui->tGridDraw->update();
#else
    Q_UNUSED(index)
#endif
}

void  MainWindow::onChangeGrid( int  index )
{
#if 0
    if( 0 == index )
        m_uGridType = keGridTypeNormal;
    else if( 1 == index )
        m_uGridType = keGridTypeShift;

    glb().m_uGridType = m_uGridType;

    ui->tGridDraw->update();
#else
    Q_UNUSED(index)
#endif
}

void  MainWindow::onUndoFilled(bool filled) {
    ui->actionUndo->setEnabled(filled);
}

void  MainWindow::onRedoFilled(bool filled) {
    ui->actionRedo->setEnabled(filled);
}

//------------------------------------------------------------------------------

void  MainWindow::showInfoMessage(const QString& msg_text, const QString& msg_info_text) {
    QMessageBox  msgBox;

    msgBox.setWindowTitle("Информация");
    msgBox.setText(msg_text);
    msgBox.setInformativeText(msg_info_text);
    msgBox.exec();
}

bool  MainWindow::askSaveIfChanged()
{
    bool result = true;

    if( m_bPrgTitleChanged )
    {
        QMessageBox     msgBox;

        msgBox.setWindowTitle("Информация");
        msgBox.setText( "Изображение не сохранено!" );
        msgBox.setInformativeText( "Хотите сохранить?" );
        msgBox.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );
        msgBox.setDefaultButton( QMessageBox::Save );

        msgBox.setButtonText(QMessageBox::Save, "Сохранить");
        msgBox.setButtonText(QMessageBox::Discard, "Не сохранять");
        msgBox.setButtonText(QMessageBox::Cancel, "Отмена");

        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
                // пробуем сохранить
                if(m_zPrgFileName.isEmpty()) {
                    result = fileSaveAs();
                }
                else {
                    result = fileSave();
                }
                //onBtnSave();
                break;

            case QMessageBox::Discard:
                // выходим без сохранения
                result = true;
                break;

            case QMessageBox::Cancel:
                // отбой
                result = false;
                break;

            default:
                break;
        }
    }

    return result;
}

//------------------------------------------------------------------------------

void  MainWindow::closeEvent( QCloseEvent  *event )
{
    if(!askSaveIfChanged()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void  MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    //qDebug() << "win" << event->size().width() << event->size().height();

    // новые размеры лейбла
//    int w = ui->lblPicture->width();
//    int h = ui->lblPicture->height();

    // если есть картинка
//    if( !m_pPixmap->isNull() )
//    {
//        // ставим отмасштабированную картинку в лэйбл
//        ui->lblPicture->setPixmap((*m_pPixmap).scaled(w, h, Qt::KeepAspectRatio));
//    }
}

//------------------------------------------------------------------------------

// количество рядов/колонок от спинбоксов
// !!! сейчас настойка делается через диалог "Новая сетка"
// -->
void MainWindow::on_spinRow_valueChanged(int arg1)
{
    m_uRow = static_cast<unsigned>(arg1);

    m_pGrid->setRows(arg1);

    ui->tGridDraw->update();
}

void MainWindow::on_spinColumn_valueChanged(int arg1)
{
    m_uColumn = static_cast<unsigned>(arg1);

    m_pGrid->setColumns(arg1);

    ui->tGridDraw->update();
}
// <--

// ловим нажатие кнопок минус/плюс ряд/колонка
// -->
void MainWindow::on_btnRowM_clicked()
{
    m_uRow--;

    if(m_uRow < MIN_ROW_COUNT) m_uRow = MIN_ROW_COUNT;

    ui->spinRow->setValue(static_cast<int>(m_uRow));

    m_pGrid->setRows(m_uRow);

    setStateChanged();

    ui->tGridDraw->update();
}

void MainWindow::on_btnRowP_clicked()
{
    m_uRow++;

    if(m_uRow > MAX_ROW_COUNT) m_uRow = MAX_ROW_COUNT;

    ui->spinRow->setValue(static_cast<int>(m_uRow));

    m_pGrid->setRows(m_uRow);

    setStateChanged();

    ui->tGridDraw->update();
}

void MainWindow::on_btnColumnM_clicked()
{
    m_uColumn--;

    if(m_uColumn < MIN_COLUMN_COUNT) m_uColumn = MIN_COLUMN_COUNT;

    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    m_pGrid->setColumns(m_uColumn);

    setStateChanged();

    ui->tGridDraw->update();
}

void MainWindow::on_btnColumnP_clicked()
{
    m_uColumn++;

    if(m_uColumn > MAX_COLUMN_COUNT) m_uColumn = MAX_COLUMN_COUNT;

    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    m_pGrid->setColumns(m_uColumn);

    setStateChanged();

    ui->tGridDraw->update();
}
// <--

// дополнительные настройки
// -->
void MainWindow::on_checkBoxGrid_stateChanged(int arg1)
{
    m_pGrid->setBorder(Qt::Unchecked != arg1);

    ui->tGridDraw->update();
}

void MainWindow::on_checkBoxGridRuler_stateChanged(int arg1)
{
    m_pGrid->setRulerBorder(Qt::Unchecked != arg1);

    ui->tGridDraw->update();
}

void MainWindow::on_checkBoxSplit_stateChanged(int arg1)
{
    m_pGrid->setSplit(Qt::Unchecked != arg1);

    ui->tGridDraw->update();
}

void MainWindow::on_checkBoxRulerV_stateChanged(int arg1)
{
    ui->radioRulerV1->setEnabled(Qt::Unchecked != arg1);
    ui->radioRulerV2->setEnabled(Qt::Unchecked != arg1);

    m_pGrid->setRulerV(Qt::Unchecked != arg1);

    ui->tGridDraw->update();

    if(Qt::Unchecked == arg1) {
        ui->radioRulerV1->setChecked(true);
    }
}

void MainWindow::on_checkBoxRulerH_stateChanged(int arg1)
{
    ui->radioRulerH1->setEnabled(Qt::Unchecked != arg1);
    //ui->radioRulerH2->setEnabled(Qt::Unchecked != arg1);

    m_pGrid->setRulerH(Qt::Unchecked != arg1);

    ui->tGridDraw->update();

    if(Qt::Unchecked == arg1) {
        ui->radioRulerH1->setChecked(true);
    }
}

void MainWindow::on_radioRulerV1_clicked()
{
    m_pGrid->setRulerVtype(keRulerTypeRight);

    ui->tGridDraw->update();
}

void MainWindow::on_radioRulerV2_clicked()
{
    m_pGrid->setRulerVtype(keRulerTypeRightLeft);

    ui->tGridDraw->update();
}

void MainWindow::on_radioRulerH1_clicked()
{
    m_pGrid->setRulerHtype(keRulerTypeBottom);

    ui->tGridDraw->update();
}

void MainWindow::on_radioRulerH2_clicked()
{
    m_pGrid->setRulerHtype(keRulerTypeBottomTop);

    ui->tGridDraw->update();
}
// <--
