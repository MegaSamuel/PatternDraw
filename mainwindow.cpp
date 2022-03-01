#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

#include <cassert>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "griddraw.h"
#include "global.h"
#include "gridsave.h"

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_ptConfig = new TConfig();

    m_bPrgTitleChanged = false;

    m_zPrgName = "PatternDraw";

    initGuiElements();

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
    // отправка сигнала об изменении цвета сетки
    connect(this, &MainWindow::changeGridColor, ui->tGridDraw, &TGridDraw::onChangeGridColor);
    // отправка сигнала об изменении цвета фона
    connect(this, &MainWindow::changeBackColor, ui->tGridDraw, &TGridDraw::onChangeBackColor);

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

    // ловим нажатие кнопки Изменить цвет сетки
    connect(ui->btnGridColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeGridColor);
    // ловим нажатие кнопки Изменить цвет элемента
    connect(ui->btnItemColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeItemColor);
    // ловим нажатие кнопки Изменить цвет фона
    connect(ui->btnBackColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeBackColor);

    // меню Файл
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewHandler);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenHandler);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveHandler);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAsHandler);
    connect(ui->actionConvert, &QAction::triggered, this, &MainWindow::onConvertHandler);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::onPrintHandler);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuitHandler);

    // меню Правка
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndoHandler);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedoHandler);

    // вызов руководства пользователя
    connect(ui->actionMan, &QAction::triggered, this, &MainWindow::onManHandler);

    // вызов справки о программе
    connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::onInfoHandler);

    // центральный элемент
    setCentralWidget( ui->centralWidget );

    // центруем лайбл с превью
    //ui->lblPicture->setAlignment( Qt::AlignCenter );

    // включаем фильтр для фрейма с таблицей
    ui->framePreviewGrid->installEventFilter(this);

    // читаем конфиг
    actionAfterStart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::initGuiElements() {
    m_uRow = static_cast<unsigned>(glb().tGridData.nRow);
    m_uColumn = static_cast<unsigned>(glb().tGridData.nColumn);

    m_uCurrRow = 1;
    m_uCurrColumn = 1;

    m_uItemType = static_cast<unsigned>(glb().tGridData.nItemType);
    m_uGridType = static_cast<unsigned>(glb().tGridData.nGridType);

    ui->spinRow->setValue(static_cast<int>(m_uRow));
    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    ui->labelCurrRow->setText("Ряд:");
    ui->labelCurrColumn->setText("Петля:");

    // цвет сетки
    m_tGridColor = Qt::gray;
    glb().tGridColor = Qt::gray;
    setLabelBackColor(ui->labelGridColor, &m_tGridColor);

    // цвет элемента
    m_tItemColor = Qt::white;
    glb().tItemColor = Qt::white;
    setLabelBackColor(ui->labelItemColor, &m_tItemColor);

    // цвет фона
    m_tBackColor = Qt::white;
    glb().tBackColor = Qt::white;
    setLabelBackColor(ui->labelBackColor, &m_tBackColor);

    ui->radioRulerH1->setChecked(true);
    ui->radioRulerV1->setChecked(true);
}

void  MainWindow::initOpenGuiElements(const t_grid_data& grid) {
    glb().tGridData.nRow = grid.row;
    glb().tGridData.nColumn = grid.column;

    glb().tGridData.nItemType = grid.item_type;
    glb().tGridData.nGridType = grid.grid_type;

    m_uRow = static_cast<unsigned>(glb().tGridData.nRow);
    m_uColumn = static_cast<unsigned>(glb().tGridData.nColumn);

    m_uCurrRow = 1;
    m_uCurrColumn = 1;

    m_uItemType = static_cast<unsigned>(glb().tGridData.nItemType);
    m_uGridType = static_cast<unsigned>(glb().tGridData.nGridType);

    ui->spinRow->setValue(static_cast<int>(m_uRow));
    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    ui->labelCurrRow->setText("Ряд:");
    ui->labelCurrColumn->setText("Петля:");

    // цвет сетки
    convFromColor(grid.t_grid_color, m_tGridColor);
    convFromColor(grid.t_grid_color, glb().tGridColor);
    setLabelBackColor(ui->labelGridColor, &m_tGridColor);

    // цвет элемента
    m_tItemColor = Qt::white;
    glb().tItemColor = Qt::white;
    setLabelBackColor(ui->labelItemColor, &m_tItemColor);

    // цвет фона
    convFromColor(grid.t_back_color, m_tBackColor);
    convFromColor(grid.t_back_color, glb().tBackColor);
    setLabelBackColor(ui->labelBackColor, &m_tBackColor);

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

bool  MainWindow::fileSavePictToDev(const QString& filename) {
    bool result;
    QString  format = filename.right(3).toUpper();

    result = ui->tGridDraw->saveImage(filename, format.toStdString().c_str());

    resetStateChanged();

    if(!result) {
        qDebug() << "Ошибка записи в файл";
    }

    return result;
}

bool  MainWindow::fileSaveConvertedToDev(const QString& filename) {
    bool result;
    QString  format = filename.right(3).toUpper();

    result = ui->tGridDraw->saveImageConverted(filename, format.toStdString().c_str());

    if(!result) {
        qDebug() << "Ошибка записи в файл";
    }

    return result;
}

bool  MainWindow::fileSave() {
    bool result = false;

    if(!m_zPrgFileName.isEmpty()) {
        if("pdg" == m_zPrgFileName.right(3))
            result = fileSaveGridToDev(m_zPrgFileName); // сохраняем во внутреннем формате
        else
            result = fileSavePictToDev(m_zPrgFileName); // сохраняем изображение
    }

    return result;
}

bool  MainWindow::fileSaveAs() {
    bool result = false;

    // формируем имя файла по умолчанию
    QString deffilename = QString("/pattern%1x%2").arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir(m_ptConfig->cfgGetSavePath() + deffilename);

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // формируем путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, "Сохранить файл",
                       QDir::toNativeSeparators(dir), "PatterDraw Grid (*.pdg);;PNG (*.png);;JPEG (*.jpg);;Bitmap picture (*.bmp)"));

    QApplication::processEvents();

    if(!filename.isEmpty()) {
        m_zPrgFileName = filename;

        if("pdg" == filename.right(3))
            result = fileSaveGridToDev(m_zPrgFileName); // сохраняем во внутреннем формате
        else
            result = fileSavePictToDev(m_zPrgFileName); // сохраняем изображение

        // запоминаем каталог
        m_ptConfig->cfgSetSavePath(m_zPrgFileName.section(QDir::separator(), 0, -2));

        // обновляем конфиг
        m_ptConfig->writeSettings();

        // заголовок формы
        setPrgTitleText(filename);
    } else {
        qDebug() << "no filename";
    }

    return result;
}

bool  MainWindow::fileSaveConverted() {
    bool result = false;

    // формируем имя файла по умолчанию
    QString deffilename = QString("/converted%1x%2").arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir(m_ptConfig->cfgGetConvPath() + deffilename);

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // формируем путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, "Сохранить файл",
                       QDir::toNativeSeparators(dir), "PNG (*.png);;JPEG (*.jpg);;Bitmap picture (*.bmp)"));

    QApplication::processEvents();

    if(!filename.isEmpty()) {
        result = fileSaveConvertedToDev(filename);

        // запоминаем каталог
        m_ptConfig->cfgSetConvPath(filename.section(QDir::separator(), 0, -2));

        // обновляем конфиг
        m_ptConfig->writeSettings();

    } else {
        qDebug() << "no filename";
    }

    return result;
}

//------------------------------------------------------------------------------

// перегрузка для t_color
inline QDataStream &operator <<(QDataStream &stream, const t_color &color) {
    stream << color.red;
    stream << color.green;
    stream << color.blue;

    return stream;
}

inline QDataStream &operator >>(QDataStream &stream, t_color &color) {
    stream >> color.red;
    stream >> color.green;
    stream >> color.blue;

    return stream;
}

// перегрузка для t_data_grid
inline QDataStream &operator <<(QDataStream &stream, const t_grid_data &grid) {
    stream << grid.id[0];
    stream << grid.id[1];
    stream << grid.id[2];
    stream << grid.item_type;
    stream << grid.grid_type;
    stream << grid.row;
    stream << grid.column;
    stream << grid.is_filled;
    stream << grid.t_grid_color;
    stream << grid.t_back_color;

    for(int i = 0; i < grid.row; i++) {
        for(int j = 0; j < grid.column; j++) {
            stream << *(grid.grid+i*grid.column+j);
        }
    }

    return stream;
}

inline QDataStream &operator >>(QDataStream &stream, t_grid_data &grid) {
    stream >> grid.id[0];
    stream >> grid.id[1];
    stream >> grid.id[2];
    stream >> grid.item_type;
    stream >> grid.grid_type;
    stream >> grid.row;
    stream >> grid.column;
    stream >> grid.is_filled;
    stream >> grid.t_grid_color;
    stream >> grid.t_back_color;

    // берем память под таблицу
    grid.grid = new t_color[grid.row*grid.column]();

    for(int i = 0; i < grid.row; i++) {
        for(int j = 0; j < grid.column; j++) {
            stream >> *(grid.grid+i*grid.column+j);
        }
    }

    return stream;
}

void  MainWindow::onOpenCreate(const t_grid_data& grid, const QString& filename) {
    m_zPrgTitle.clear();

    m_bPrgTitleChanged = false;

    m_zPrgFileName.clear();

    initOpenGuiElements(grid);

    glb().pGrid->initCells(grid);

    ui->tGridDraw->setVisible(true);

    // сформировано ли изображение
    m_bImageReady = true;

    guiBlock(false);

    // заголовок формы
    setPrgTitleText(filename);

    // если сетка со смещением и из прямоугольников, то ее можно конвертировать
    if((keGridTypeShift == m_uGridType) && (keItemTypeRectan == m_uItemType)) {
        ui->actionConvert->setEnabled(true);
    }
}

bool  MainWindow::fileOpenGridFromDev(const QString& filename) {
    bool result = false;

    QFile 		fp(filename);

    result = fp.open(QFile::ReadOnly);

    QDataStream out(&fp);

    out.setVersion(QDataStream::Qt_5_12);

    t_grid_data filedata;

    out >> filedata;

    fp.close();

    //TODO необходимо проверить формат файла (первые три байта должны быть "PDG")

    if(!result) {
        qDebug() << "Ошибка чтения из файла";
    } else {
        onOpenCreate(filedata, filename);
        delete [] filedata.grid;
    }

    return result;
}

bool  MainWindow::fileOpenGrid() {
    bool result = false;

    // каталог где мы находимся
    QDir *pDir = new QDir(m_ptConfig->cfgGetOpenPath());

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // формируем путь и имя файла через диалог
    QString filename = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, "Открыть файл",
                       QDir::toNativeSeparators(dir), "PatterDraw Grid (*.pdg)"));

    QApplication::processEvents();

    if(!filename.isEmpty()) {
        result = fileOpenGridFromDev(filename);

        // ставим имя, чтобы работала кнопка Сохранить
        m_zPrgFileName = filename;

        // запоминаем каталог
        m_ptConfig->cfgSetOpenPath(filename.section(QDir::separator(), 0, -2));

        // обновляем конфиг
        m_ptConfig->writeSettings();
    } else {
        qDebug() << "no filename";
    }

    return result;
}

bool  MainWindow::fileSaveGridToDev(const QString& filename) {
    std::string str;
    QFile 		fp(filename);
    QDataStream out(&fp);

    // открываем файл на запись
    if(!fp.open(QIODevice::WriteOnly))
    {
        qDebug() << "cannot create file";
        return false;
    }

    t_grid_data filedata;

    filedata.id[0] = static_cast<uint8_t>('P');
    filedata.id[1] = static_cast<uint8_t>('D');
    filedata.id[2] = static_cast<uint8_t>('G');
    filedata.item_type = static_cast<uint8_t>(m_uItemType);
    filedata.grid_type = static_cast<uint8_t>(m_uGridType);

    filedata.row = static_cast<uint8_t>(m_uRow);
    filedata.column = static_cast<uint8_t>(m_uColumn);

    filedata.is_filled = static_cast<uint8_t>(m_tBackColor == Qt::white ? false : true);

    convToColor(&filedata.t_grid_color, m_tGridColor);
    convToColor(&filedata.t_back_color, m_tBackColor);

    // берем память под таблицу
    filedata.grid = new t_color[filedata.row*filedata.column]();

    for(int i = 0; i < filedata.row; i++) {
        for(int j = 0; j < filedata.column; j++) {
            convToColor(filedata.grid+i*filedata.column+j, m_pGrid->getElement(i, j).getFillColor());
        }
    }

    // пишем в файл
    out.setVersion(QDataStream::Qt_5_12);
    out << filedata;
    fp.flush();
    fp.close();

    // возвращаем память
    delete [] filedata.grid;

    resetStateChanged();

    return true;
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
    QString txt;

    if(row > 0) {
        txt = "Ряд: " + QString::number(row);
        m_uCurrRow = static_cast<unsigned>(row);
    } else {
        txt = "Ряд:";
        m_uCurrRow = 0;
    }

    ui->labelCurrRow->setText(txt);

    if(col > 0) {
        txt = "Петля: " + QString::number(col);
        m_uCurrColumn = static_cast<unsigned>(col);
    } else {
        txt = "Петля:";
        m_uCurrColumn = 0;
    }

    ui->labelCurrColumn->setText(txt);
}

void  MainWindow::onChangeState() {
    setStateChanged();
}

void  MainWindow::onBtnChangeGridColor()
{
    QColor color;

    //!bug почему-то не работает
    m_tColorDialog.setCurrentColor(m_tGridColor);

    color = m_tColorDialog.getColor();

    if(color.isValid())
    {
        m_tGridColor = color;

        setLabelBackColor(ui->labelGridColor, &m_tGridColor);

        glb().tGridColor = color;

        Q_EMIT(changeGridColor(color));
    }
}

void  MainWindow::onBtnChangeItemColor()
{
    QColor color;

    //!bug почему-то не работает
    m_tColorDialog.setCurrentColor(m_tItemColor);

    color = m_tColorDialog.getColor();

    if(color.isValid())
    {
        m_tItemColor = color;

        setLabelBackColor(ui->labelItemColor, &m_tItemColor);

        glb().tItemColor = color;
    }
}

void  MainWindow::onBtnChangeBackColor()
{
    QColor color;

    //!bug почему-то не работает
    m_tColorDialog.setCurrentColor(m_tBackColor);

    color = m_tColorDialog.getColor();

    if(color.isValid())
    {
        m_tBackColor = color;

        setLabelBackColor(ui->labelBackColor, &m_tBackColor);

        glb().tBackColor = color;

        Q_EMIT(changeBackColor(color));
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

    // если сетка со смещением и из прямоугольников, то ее можно конвертировать
    if((keGridTypeShift == m_uGridType) && (keItemTypeRectan == m_uItemType)) {
        ui->actionConvert->setEnabled(true);
    }

    // если область была большой - уменьшаем ее
    //TODO криво, нужно переделать
    if((ui->tGridDraw->size().height()+2 > ui->framePreviewGrid->size().height()) ||
       (ui->tGridDraw->size().width()+2 > ui->framePreviewGrid->size().width())) {
        ui->tGridDraw->setMinimumWidth(ui->framePreviewGrid->size().width()-2);
        ui->tGridDraw->setMinimumHeight(ui->framePreviewGrid->size().height()-2);
    }
}

void  MainWindow::onOpenHandler() {
    // есть несохраненные изменения
    if(!askSaveIfChanged()) {
        return;
    }

    fileOpenGrid();
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

void  MainWindow::onConvertHandler() {
    // нет картинки
    if(!m_bImageReady) {
        showInfoMessage("Нет изображения!", "Перед конвертацией создайте новую сетку.");
        return;
    }

    fileSaveConverted();
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
    msgBox.setText(msg_text + "\n\n" + msg_info_text);
    //msgBox.setInformativeText(msg_info_text);
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

bool  MainWindow::eventFilter(QObject *object, QEvent *event) {
    if(object == ui->framePreviewGrid){
//        if(event->type() == QEvent::Enter) {
//            qDebug() << "enter";
//            return true;
//        }
        if(event->type() == QEvent::Leave) {
            // если покидаем область, то сбрасываем показания ряд/петля
            onCurrentPos(-1, -1);
            return true;
        }
    }

    return QMainWindow::eventFilter(object, event);
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

//------------------------------------------------------------------------------

void  MainWindow::actionAfterStart() {
    // каталог где мы находимся
    QDir *pDir = new QDir(QDir::currentPath());

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // установка дефолтного пути
    m_ptConfig->cfgSetCurrentPath(QDir::toNativeSeparators(dir));

    // читаем конфиг
    m_ptConfig->readSettings();

    delete pDir;
}

//------------------------------------------------------------------------------
