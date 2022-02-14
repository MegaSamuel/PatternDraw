#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>

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

    initGuiElements();

    // диалог Новыя сетка
    m_ptNewDialog = new TNewDialog(this);
    connect(m_ptNewDialog, &TNewDialog::dlgCreate, this, &MainWindow::onDlgCreate);

    // картинка для превью
    m_pPixmap = new QPixmap;

    // изображение для превью
    m_pImage = new QByteArray;

    // таблица элементов
    m_pGrid = new TGrid(static_cast<int>(m_uRow), static_cast<int>(m_uColumn));

    ui->tGridDraw->setVisible(false);

    // отправляем указатель на таблицу в рисовалку
    glb().pGrid = m_pGrid;

    // сформировано ли изображение
    m_bImageReady = false;

    guiBlock(true);

    setCellSize();

    // ставим стиль
    QApplication::setStyle(QStyleFactory::create("fusion"));

    // заголовок формы
    setPrgTitleText();

    // иконка формы
    setWindowIcon( QIcon( ":/PatternDraw.ico" ) );

    // ловим нажатие кнопки Сохранить
    //connect( ui->btnSave, &QPushButton::clicked, this, &MainWindow::onBtnSave );

    // ловим нажатие кнопки Предпросмотр
    connect( ui->btnPreview, &QPushButton::clicked, this, &MainWindow::onBtnPreview );

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

    // комбобокс с размером элемента
    //connect( ui->comboBoxSize, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onChangeSize );

    // комбобокс с типом строки
    connect( ui->comboBoxGrid, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onChangeGrid );

    // центральный элемент
    setCentralWidget( ui->centralWidget );

    // центруем лайбл с превью
    ui->lblPicture->setAlignment( Qt::AlignCenter );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::initGuiElements() {
    m_uRow = static_cast<unsigned>(glb().tGridData.nRow);
    m_uColumn = static_cast<unsigned>(glb().tGridData.nColumn);

    m_uItemType = static_cast<unsigned>(glb().tGridData.nItemType);
    m_uGridType = static_cast<unsigned>(glb().tGridData.nGridType);

    ui->spinRow->setValue(static_cast<int>(m_uRow));
    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

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

    ui->checkRulerH1->setChecked(true);
    ui->checkRulerH2->setChecked(false);

    ui->checkRulerV1->setChecked(true);
    ui->checkRulerV2->setChecked(false);
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

bool  MainWindow::imageFillShift()
{
    bool  result = false;

    RGBQUAD color;
    QDataStream  stream( m_pImage, QIODevice::WriteOnly );
    int div_i_end = 0, div_i_begin = 0;
    int div_i_odd_end = 0, div_i_odd_begin = 0;
    int div_j_end = 0, div_j_begin = 0;
    int row = 0, row_odd = 0, column = 0;

    // подгоняем массив под размер изображения
    m_pImage->resize( static_cast<int>(m_tBitMap.bfh.bfSize) );

    // обнуляем массив
    memset( m_pImage->data(), 0, static_cast<size_t>(m_tBitMap.bfh.bfSize) );

    stream.setVersion(QDataStream::Qt_5_12);

    // пишем заголовок картинки
    stream.writeRawData( reinterpret_cast<char*>(&m_tBitMap), sizeof(TBitMap) );

    // картинка заполняется слева направо снизу вверх.

    // сброс счетчиков
    row = 0;
    row_odd = 0;

    // проход по высоте (по строкам)
    for( int i = 0; i < m_tBitMap.bih.biHeight; i++ )
    {
        if( m_uRow%2 )
        {
            // нечетное количество строк

            // нечетные столбцы
            // признак первого пикселя ячейки по высоте
            div_i_odd_begin = i % static_cast<int>(m_tCell.h);
            // признак последнего пикселя ячейки по высоте
            div_i_odd_end = (i+1) % static_cast<int>(m_tCell.h);

            // четные столбцы
            // признак первого пикселя ячейки по высоте
            div_i_begin = (i + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);
            // признак последнего пикселя ячейки по высоте
            div_i_end = (i+1 + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);
        }
        else
        {
            // четное количество строк

            // нечетные столбцы
            // признак первого пикселя ячейки по высоте
            div_i_odd_begin = (i + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);
            // признак последнего пикселя ячейки по высоте
            div_i_odd_end = (i+1 + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);

            // четные столбцы
            // признак первого пикселя ячейки по высоте
            div_i_begin = i % static_cast<int>(m_tCell.h);
            // признак последнего пикселя ячейки по высоте
            div_i_end = (i+1) % static_cast<int>(m_tCell.h);
        }

        // номер текущей строки (для нечетных столбцов)
        if( row_odd <= static_cast<int>(m_uRow) )
        {
            if( 0 == div_i_odd_begin )
                row_odd += 1;
        }
        else
        {
            row_odd = 0;
        }

        if( m_uRow%2 )
        {
            // нечетное количество строк

            // номер текущей строки (для четных столбцов)
            if( ( 1 != m_uRow ) && ( row <= static_cast<int>((m_uRow-1)/2) ) )
            {
                if( 0 == div_i_begin )
                    row += 1;
            }
            else
            {
                row = 0;
            }
        }
        else
        {
            // четное количество строк

            // номер текущей строки (для четных столбцов)
            if( row <= static_cast<int>(m_uRow/2) )
            {
                if( 0 == div_i_begin )
                    row += 1;
            }
            else
            {
                row = 0;
            }
        }

        // сброс счетчика
        column = 1;

        // проход по ширине (по столбцам)
        for( int j = 0; j < m_tBitMap.bih.biWidth; j++ )
        {
            // признак первого пикселя ячейки по ширине
            div_j_begin = j % static_cast<int>(m_tCell.w);
            // признак последнего пикселя ячейки по ширине
            div_j_end = (j+1) % static_cast<int>(m_tCell.w);

            // номер текущего столбца
            if( 0 == j )
                column = 1;
            else if( 0 == div_j_begin )
                column += 1;

            // цвет фона
            getBackColor( &color );

            // рисуем нечетные столбцы
            if( row_odd )
            {
                if( (column)%2 )
                {
                    if( ( 0 == div_i_odd_begin ) || ( 0 == div_i_odd_end ) ||
                        ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // цвет сетки
                        getGridColor( &color );
                    }
                }
            }

            // рисуем четные столбцы
            if( row )
            {
                if( (column-1)%2 )
                {
                    if( ( 0 == div_i_begin ) || ( 0 == div_i_end ) ||
                        ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // цвет сетки
                        getGridColor( &color );
                    }
                }
            }

            // пишем пиксель
            stream.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }

    if( stream.status() != QDataStream::Ok )
    {
        qDebug() << "Ошибка записи";
    }
    else
    {
        result = true;
    }

    return result;
}

bool  MainWindow::imageFillNormal()
{
    bool  result = false;

    RGBQUAD      color;
    QDataStream  stream( m_pImage, QIODevice::WriteOnly );
#if 0
    int  div_i_begin = 0;
    int  div_j_begin = 0;

    // подгоняем массив под размер изображения
    m_pImage->resize( static_cast<int>(m_tBitMap.bfh.bfSize) );

    // обнуляем массив
    memset( m_pImage->data(), 0, static_cast<size_t>(m_tBitMap.bfh.bfSize) );

    stream.setVersion(QDataStream::Qt_5_12);

    // пишем заголовок картинки
    stream.writeRawData( reinterpret_cast<char*>(&m_tBitMap), sizeof(TBitMap) );

    // картинка заполняется слева направо снизу вверх.

    // проход по высоте (по строкам)
    for( int i = 0; i < m_tBitMap.bih.biHeight; i++ )
    {
        // признак первого пикселя ячейки по высоте
        div_i_begin = i % static_cast<int>(m_tCell.h-1);

        // проход по ширине (по столбцам)
        for( int j = 0; j < m_tBitMap.bih.biWidth; j++ )
        {
            // признак первого пикселя ячейки по ширине
            div_j_begin = j % static_cast<int>(m_tCell.w-1);

            // цвет фона
            getBackColor( &color );

            // рисуем столбцы
            if( ( 0 == div_i_begin ) || ( 0 == div_j_begin ) )
            {
                // цвет сетки
                getGridColor( &color );
            }

            // пишем пиксель
            stream.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }
#else
    int  div_i_end = 0, div_i_begin = 0;
    int  div_j_end = 0, div_j_begin = 0;

    // подгоняем массив под размер изображения
    m_pImage->resize( static_cast<int>(m_tBitMap.bfh.bfSize) );

    // обнуляем массив
    memset( m_pImage->data(), 0, static_cast<size_t>(m_tBitMap.bfh.bfSize) );

    stream.setVersion(QDataStream::Qt_5_12);

    // пишем заголовок картинки
    stream.writeRawData( reinterpret_cast<char*>(&m_tBitMap), sizeof(TBitMap) );

    // картинка заполняется слева направо снизу вверх.

    // проход по высоте (по строкам)
    for( int i = 0; i < m_tBitMap.bih.biHeight; i++ )
    {
        // признак первого пикселя ячейки по высоте
        div_i_begin = i % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_end = (i+1) % static_cast<int>(m_tCell.h);

        // проход по ширине (по столбцам)
        for( int j = 0; j < m_tBitMap.bih.biWidth; j++ )
        {
            // признак первого пикселя ячейки по ширине
            div_j_begin = j % static_cast<int>(m_tCell.w);
            // признак последнего пикселя ячейки по ширине
            div_j_end = (j+1) % static_cast<int>(m_tCell.w);

            // цвет фона
            getBackColor( &color );

            // рисуем столбцы
            if( ( 0 == i ) || ( 0 == div_i_begin ) || ( 0 == div_i_end ) ||
                ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
            {
                // цвет сетки
                getGridColor( &color );
            }

            // пишем пиксель
            stream.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }
#endif
    if( stream.status() != QDataStream::Ok )
    {
        qDebug() << "Ошибка записи";
    }
    else
    {
        result = true;
    }

    return result;
}

bool  MainWindow::imageCreate()
{
    RGBQUAD   color;
    unsigned  uWidth = 0;
    unsigned  uHeight = 0;

    // размер картинки в пикселях
#if 0
    if( keGridTypeShift == m_uGridType )
    {
        uWidth = m_uColumn * m_tCell.w - ( m_uColumn - 1 );
        uHeight = static_cast<unsigned>( m_uRow / 2.0 * m_tCell.h + m_tCell.h / 2.0 );
    }
    else
    {
        uWidth = m_uColumn * m_tCell.w - ( m_uColumn - 1 );
        uHeight = m_uRow * m_tCell.h - ( m_uRow - 1 );
    }
#else
    if( keGridTypeShift == m_uGridType )
    {
        uWidth = m_uColumn * m_tCell.w;
        uHeight = static_cast<unsigned>( m_uRow / 2.0 * m_tCell.h + m_tCell.h / 2.0 );
    }
    else
    {
        uWidth = m_uColumn * m_tCell.w;
        uHeight = m_uRow * m_tCell.h;
    }
#endif
    // очищаем file header
    memset( &m_tBitMap.bfh, 0, sizeof(m_tBitMap.bfh) );

    // заполняем file header
    m_tBitMap.bfh.bfType = 0x4D42;
    m_tBitMap.bfh.bfOffBits = sizeof(m_tBitMap.bfh) + sizeof(m_tBitMap.bih) + sizeof(m_tBitMap.auColorTable);
    m_tBitMap.bfh.bfSize = m_tBitMap.bfh.bfOffBits + sizeof(color) * uWidth * uHeight;
    m_tBitMap.bfh.bfReserved1 = 0;
    m_tBitMap.bfh.bfReserved2 = 0;

    // очищаем info header
    memset( &m_tBitMap.bih, 0, sizeof(m_tBitMap.bih) );

    // заполняем info header
    m_tBitMap.bih.biSize = sizeof(m_tBitMap.bih);
    m_tBitMap.bih.biWidth = static_cast<LONG>(uWidth);
    m_tBitMap.bih.biHeight = static_cast<LONG>(uHeight);
    m_tBitMap.bih.biPlanes = 1;
    m_tBitMap.bih.biBitCount = 32;
    m_tBitMap.bih.biCompression = 0;
    m_tBitMap.bih.biSizeImage = 0;
    m_tBitMap.bih.biXPelsPerMeter = 0;
    m_tBitMap.bih.biYPelsPerMeter = 0;
    m_tBitMap.bih.biClrUsed = 0;
    m_tBitMap.bih.biClrImportant = 0;

    // очищаем color table
    memset( &m_tBitMap.auColorTable, 0, sizeof(m_tBitMap.auColorTable) );

    // формируем саму картинку
    if( keGridTypeShift == m_uGridType )
    {
        m_bImageReady = imageFillShift();
    }
    else
    {
        m_bImageReady = imageFillNormal();
    }

    if( !m_bPrgTitleChanged )
    {
        setPrgTitleChanged( true );
    }

    m_bPrgTitleChanged = m_bImageReady;

    return m_bImageReady;
}

//------------------------------------------------------------------------------

bool  MainWindow::fileSave() {
    bool result = false;
    // собственно файл
    QFile file;

    // формируем картинку
    if(imageCreate()) {
        if(0 != m_pImage->size()) {
            m_pPixmap->loadFromData(*m_pImage, "BMP");
        }
    } else {
        qDebug() << "cannot create image";
        return false;
    }

    if(!m_zPrgFileName.isEmpty()) {
        file.setFileName(m_zPrgFileName);

        if(!file.open(QIODevice::WriteOnly)) {
            qDebug() << "cannot open file" << m_zPrgFileName;
            return false;
        } else {
            QString  format = m_zPrgFileName.right(3).toUpper();

            if(m_pPixmap->save(&file, format.toStdString().c_str())) {
                result = true;
                if(m_bPrgTitleChanged) {
                    setPrgTitleChanged(false);
                }
                m_bPrgTitleChanged = false;
            } else {
                qDebug() << "Ошибка записи в файл";
            }

            file.close();
        }
    }

    return result;
}

bool  MainWindow::fileSaveAs() {
    bool result = false;
    // собственно файл
    QFile file;

    // формируем картинку
    if(imageCreate()) {
        if(0 != m_pImage->size()) {
            m_pPixmap->loadFromData( *m_pImage, "BMP" );
        }
    } else {
        qDebug() << "cannot create image";
        return false;
    }

    // формируем имя файла по умолчанию
    QString deffilename = QString("/pattern%1x%2").arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir(QDir::currentPath() + deffilename);

    // строка с именем каталога где мы находимся
    QString dir(pDir->path());

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", dir, "Изображение в формате PNG (*.png);;Изображение в формате BMP (*.bmp)");

    QApplication::processEvents();

    if(!filename.isEmpty()) {
        file.setFileName(filename);

        m_zPrgFileName = filename;

        if(!file.open(QIODevice::WriteOnly)) {
            qDebug() << "cannot open file" << filename;
            return false;
        } else {
            QString  format = filename.right(3).toUpper();

            if(m_pPixmap->save(&file, format.toStdString().c_str())) {
                result = true;
                if(m_bPrgTitleChanged) {
                    setPrgTitleChanged(false);
                }
                m_bPrgTitleChanged = false;
            } else {
                qDebug() << "Ошибка записи в файл";
            }

            file.close();
        }
    } else {
        qDebug() << "no filename";
    }

    return result;
}

//------------------------------------------------------------------------------

void  MainWindow::onBtnSave()
{
    // собственно файл
    QFile file;

    // формируем картинку
    if( imageCreate() )
    {
        if( 0 != m_pImage->size() )
        {
            m_pPixmap->loadFromData( *m_pImage, "BMP" );
        }
    }
    else
    {
        qDebug() << "cannot create image";
        return;
    }

    // формируем имя файла по умолчанию
    QString deffilename = QString( "/pattern%1x%2" ).arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() + deffilename );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName( this, "Сохранить файл", dir, "Изображение в формате PNG (*.png);;Изображение в формате BMP (*.bmp)" );

    QApplication::processEvents();

    if( filename != "" )
    {
        file.setFileName( filename );

        m_zPrgFileName = filename;

        if( !file.open(QIODevice::WriteOnly) )
        {
            qDebug() << "cannot open file" << filename;

            return;
        }
        else
        {
            QString  format = filename.right(3).toUpper();

            if( m_pPixmap->save(&file, format.toStdString().c_str()) )
            {
                if( m_bPrgTitleChanged )
                {
                    setPrgTitleChanged( false );
                }
                m_bPrgTitleChanged = false;
            }
            else
            {
                qDebug() << "Ошибка записи в файл";
            }

            file.close();
        }
    }
    else
    {
        qDebug() << "no filename";
    }
}

void  MainWindow::onBtnPreview()
{
    // размеры лейбла
    int w = ui->lblPicture->width();
    int h = ui->lblPicture->height();

    // формируем картинку
    imageCreate();

    if( 0 != m_pImage->size() )
    {
        m_pPixmap->loadFromData( *m_pImage, "BMP" );
    }

    if( !m_pPixmap->isNull() )
    {
        // ставим отмасштабированную картинку в лэйбл
        ui->lblPicture->setPixmap((*m_pPixmap).scaled(w, h, Qt::KeepAspectRatio));
    }
    else
    {
       qDebug() << "there is no image!";
    }
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

        update();
    }
}

void  MainWindow::onNewHandler() {
    // есть несохраненные изменения
    if(!askSaveIfChanged("Не сохранять")) {
        return;
    }

    m_ptNewDialog->open();
}

void  MainWindow::onDlgCreate() {
    m_zPrgTitle.clear();

    m_bPrgTitleChanged = false;

    m_zPrgFileName.clear();

    initGuiElements();

    ui->tGridDraw->setVisible(true);

    // картинка для превью
    if(!m_pPixmap->isNull()) {
        //!TODO тут нужно очищать превьюшный лейбл
        //m_pPixmap = new QPixmap;
        //ui->lblPicture->setPixmap();
    }

    // изображение для превью
    if(m_pImage->size()) {
        m_pImage->clear();
    }

    // сформировано ли изображение
    m_bImageReady = false;

    guiBlock(false);

    setCellSize();

    // заголовок формы
    setPrgTitleText();
}

void  MainWindow::onOpenHandler() {

}

void  MainWindow::onSaveHandler() {
    if(m_zPrgFileName.isEmpty()) {
        fileSaveAs();
    }
    else {
        fileSave();
    }
}

void  MainWindow::onSaveAsHandler() {
    fileSaveAs();
}

void  MainWindow::onPrintHandler() {
    // нет картинки
    if(m_pPixmap->isNull()) {
        QMessageBox  msgBox;

        msgBox.setWindowTitle("Информация");
        msgBox.setText( "Нет сформированного изображения!\nПеред печатью нажмите кнопку \"Предпросмотр\"" );
        msgBox.exec();

        return;
    }

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));

    // нажали Ок
    if(dialog->exec() != QDialog::Accepted)
        return;

    // есть картинка
#if 0
    QPainter painter;
    painter.begin(&printer);
    painter.drawImage(0, 0, m_pPixmap->toImage());
    painter.end();
#else
    //!TODO сейчас картинка помещается в левый верхний угол растягивается на весь лист
    //!добавить настройки: центровка, поля, масштаб?..
    QPainter painter(&printer);
    const QImage &image = m_pPixmap->toImage();
    QRect rect = painter.viewport();
    QSize size = image.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(image.rect());
    painter.drawImage(0, 0, image);
#endif
}

void  MainWindow::onQuitHandler() {
    //!TODO сейчас хоткей ctrl+q надо бы переделать на alt+f4
    if(askSaveIfChanged()) {
        QApplication::quit();
    }
}

void  MainWindow::onUndoHandler() {

}

void  MainWindow::onRedoHandler() {

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

    setCellSize();

    update();
#else
    Q_UNUSED(index)
#endif
}

//void  MainWindow::onChangeSize( int  index )
//{
//    if( 0 == index )
//        m_uItemSize = keItemSizeSmall;
//    else if( 1 == index )
//        m_uItemSize = keItemSizeNormal;
//    else if( 2 == index )
//        m_uItemSize = keItemSizeHuge;

//    glb().m_uItemSize = m_uItemSize;

//    setCellSize();
//}

void  MainWindow::onChangeGrid( int  index )
{
#if 0
    if( 0 == index )
        m_uGridType = keGridTypeNormal;
    else if( 1 == index )
        m_uGridType = keGridTypeShift;

    glb().m_uGridType = m_uGridType;

    update();
#else
    Q_UNUSED(index)
#endif
}

//------------------------------------------------------------------------------

void  MainWindow::setCellSize()
{
    // базовый размер ячейки в пикселях

    if( keItemTypeRectan == m_uItemType )
    {
        m_tCell.h = 32;
        m_tCell.w = 16;
    }
    else if( keItemTypeSquare == m_uItemType )
    {
        m_tCell.h = 16;
        m_tCell.w = 16;
    }
    else
    {
        m_tCell.h = 2;
        m_tCell.w = 2;
    }

    // корректируем размер размер ячейки

//    if( keItemSizeSmall == m_uItemSize )
//    {
//        m_tCell.h /= 2;
//        m_tCell.w /= 2;
//    }
//    else if( keItemSizeHuge == m_uItemSize )
//    {
//        m_tCell.h *= 2;
//        m_tCell.w *= 2;
//    }
}

//------------------------------------------------------------------------------

bool  MainWindow::askSaveIfChanged(const QString& discard)
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

        msgBox.setButtonText( QMessageBox::Save, "Сохранить" );
        if(discard.isEmpty())
            msgBox.setButtonText( QMessageBox::Discard, "Выход" );
        else
            msgBox.setButtonText( QMessageBox::Discard, discard );
        msgBox.setButtonText( QMessageBox::Cancel, "Отмена" );

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
    int w = ui->lblPicture->width();
    int h = ui->lblPicture->height();

    // если есть картинка
    if( !m_pPixmap->isNull() )
    {
        // ставим отмасштабированную картинку в лэйбл
        ui->lblPicture->setPixmap((*m_pPixmap).scaled(w, h, Qt::KeepAspectRatio));
    }
}

//------------------------------------------------------------------------------

// количество рядов/колонок от спинбоксов
// !!! сейчас настойка делается через диалог "Новая сетка"
// -->
void MainWindow::on_spinRow_valueChanged(int arg1)
{
    m_uRow = static_cast<unsigned>(arg1);

    m_pGrid->setRows(arg1);

    update();
}

void MainWindow::on_spinColumn_valueChanged(int arg1)
{
    m_uColumn = static_cast<unsigned>(arg1);

    m_pGrid->setColumns(arg1);

    update();
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

    if(!m_bPrgTitleChanged) {
        m_bPrgTitleChanged = true;
        setPrgTitleChanged(true);
    }

    update();
}

void MainWindow::on_btnRowP_clicked()
{
    m_uRow++;

    if(m_uRow > MAX_ROW_COUNT) m_uRow = MAX_ROW_COUNT;

    ui->spinRow->setValue(static_cast<int>(m_uRow));

    m_pGrid->setRows(m_uRow);

    if(!m_bPrgTitleChanged) {
        m_bPrgTitleChanged = true;
        setPrgTitleChanged(true);
    }

    update();
}

void MainWindow::on_btnColumnM_clicked()
{
    m_uColumn--;

    if(m_uColumn < MIN_COLUMN_COUNT) m_uColumn = MIN_COLUMN_COUNT;

    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    m_pGrid->setColumns(m_uColumn);

    if(!m_bPrgTitleChanged) {
        m_bPrgTitleChanged = true;
        setPrgTitleChanged(true);
    }

    update();
}

void MainWindow::on_btnColumnP_clicked()
{
    m_uColumn++;

    if(m_uColumn > MAX_COLUMN_COUNT) m_uColumn = MAX_COLUMN_COUNT;

    ui->spinColumn->setValue(static_cast<int>(m_uColumn));

    m_pGrid->setColumns(m_uColumn);

    if(!m_bPrgTitleChanged) {
        m_bPrgTitleChanged = true;
        setPrgTitleChanged(true);
    }

    update();
}
// <--

// дополнительные настройки
// -->
void MainWindow::on_checkBoxGrid_stateChanged(int arg1)
{
    m_pGrid->setBorder(Qt::Unchecked != arg1);

    update();
}

void MainWindow::on_checkBoxRulerV_stateChanged(int arg1)
{
    ui->checkRulerV1->setEnabled(Qt::Unchecked != arg1);
    ui->checkRulerV2->setEnabled(Qt::Unchecked != arg1);

    m_pGrid->setRulerV(Qt::Unchecked != arg1);

    update();

    if(Qt::Unchecked == arg1) {
        ui->checkRulerV1->setChecked(true);
        ui->checkRulerV2->setChecked(false);
    }
}

void MainWindow::on_checkBoxRulerH_stateChanged(int arg1)
{
    ui->checkRulerH1->setEnabled(Qt::Unchecked != arg1);
    //ui->checkRulerH2->setEnabled(Qt::Unchecked != arg1);

    m_pGrid->setRulerH(Qt::Unchecked != arg1);

    update();

    if(Qt::Unchecked == arg1) {
        ui->checkRulerH1->setChecked(true);
        ui->checkRulerH2->setChecked(false);
    }
}

void MainWindow::on_checkRulerV1_stateChanged(int arg1)
{
    if(Qt::Unchecked != arg1) {
        ui->checkRulerV2->setChecked(false);
        m_pGrid->setRulerVtype(keRulerTypeRight);
        update();
    }
}

void MainWindow::on_checkRulerV2_stateChanged(int arg1)
{
    if(Qt::Unchecked != arg1) {
        ui->checkRulerV1->setChecked(false);
        m_pGrid->setRulerVtype(keRulerTypeRightLeft);
        update();
    }
}

void MainWindow::on_checkRulerH1_stateChanged(int arg1)
{
    if(Qt::Unchecked != arg1) {
        ui->checkRulerH2->setChecked(false);
        m_pGrid->setRulerHtype(keRulerTypeBottom);
        update();
    }
}

void MainWindow::on_checkRulerH2_stateChanged(int arg1)
{
    if(Qt::Unchecked != arg1) {
        ui->checkRulerH1->setChecked(false);
        m_pGrid->setRulerHtype(keRulerTypeBottomTop);
        update();
    }
}
// <--
