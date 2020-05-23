#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QDebug>

//------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // дефолтные значения переменных
    m_uRow = 1;
    m_uColumn = 1;

    m_uGridType = keGridTypeNormal;
    m_uItemType = keItemTypeRectan;

    ui->comboBoxItem->setCurrentIndex( static_cast<int>(m_uItemType) );
    ui->comboBoxGrid->setCurrentIndex( static_cast<int>(m_uGridType) );

    setCellSize();

    // заголовок формы
    setWindowTitle("PatternDraw");

    // иконка формы
    setWindowIcon( QIcon( ":/PatternDraw.ico" ) );

    // прячем избыточные кнопки заголовка формы
    setWindowFlag(Qt::WindowMinMaxButtonsHint,false);
    setWindowFlag(Qt::WindowSystemMenuHint,false);

    // ловим нажатие кнопки
    connect( ui->pushButton, SIGNAL(pressed()), this, SLOT(onButtonHandler()) );

    // вызов руководства пользователя
    connect( ui->actionman, SIGNAL(triggered()), SLOT(onManHandler()) );

    // вызов справки о программе
    connect( ui->actioninfo, SIGNAL(triggered()), SLOT(onInfoHandler()) );

    // комбобокс с типом элемента
    connect( ui->comboBoxItem, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeItem(int)) );

    // комбобокс с типом строки
    connect( ui->comboBoxGrid, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeGrid(int)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------------

void  MainWindow::BitMapFill( QFile  *file )
{
    RGBQUAD color;
    int div_i_end, div_i_begin = 0;
    int div_i_odd_end, div_i_odd_begin = 0;
    int div_j_end, div_j_begin = 0;
    int row = 1, row_odd = 1, column = 1;
    QDataStream dstrm(file);

    dstrm.setVersion(QDataStream::Qt_5_5);

    // пишем заголовок картинки в файл
    dstrm.writeRawData( reinterpret_cast<char*>(&m_bitmap), sizeof(BitMap) );

    int i, j;

    // картинка заполняется слева направо снизу вверх.

    // проход по высоте (по строкам)
    for(i = 0; i < m_bitmap.bih.biHeight; i++)
    {
        // четные столбцы
        // признак первого пикселя ячейки по высоте
        div_i_begin = i % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_end = (i+1) % static_cast<int>(m_tCell.h);

        // нечетные столбцы
        // признак первого пикселя ячейки по высоте
        div_i_odd_begin = (i + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_odd_end = (i+1 + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);

        // номер текущей строки (для четных столбцов)
        if( 0 == i )
            row = 1;
        else if( 0 == div_i_begin )
            row += 1;

        // номер текущей строки (для нечетных столбцов)
        if( 0 == i )
            row_odd = 1;
        else if( 0 == div_i_odd_begin )
            row_odd += 1;

        // проход по ширине (по столбцам)
        for(j = 0; j < m_bitmap.bih.biWidth; j++)
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

            // белый
            color.rgbRed = 0xFF;
            color.rgbGreen = 0xFF;
            color.rgbBlue = 0xFF;
            color.rgbReserved = 0x0;

            // рисуем четные столбцы
            if( row <= static_cast<int>(m_uRow) )
            {
                if( (column-1)%2 )
                {
                    if( ( 0 == i ) || ( 0 == div_i_begin ) || ( 0 == div_i_end ) ||
                        ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // серый
                        color.rgbRed = 0x7f;
                        color.rgbGreen = 0x7f;
                        color.rgbBlue = 0x7f;
                        color.rgbReserved = 0x0;
                    }
                }
            }

            // рисуем нечетные столбцы
            if( 1 != row_odd )
            {
                if( (column)%2 )
                {
                    if( ( 0 == i ) || ( 0 == div_i_odd_begin ) || ( 0 == div_i_odd_end ) ||
                        ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // серый
                        color.rgbRed = 0x7f;
                        color.rgbGreen = 0x7f;
                        color.rgbBlue = 0x7f;
                        color.rgbReserved = 0x0;
                    }
                }
            }

//            qDebug() << "h" << i << "w" << j << "xxx" << j+1 << div_j_begin << div_j_end;
//            qDebug() << "h" << i << "w" << j << "xxx" << row << column;

            // пишем пиксель в файл
            dstrm.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }

    if( dstrm.status() != QDataStream::Ok )
    {
        qDebug() << "Ошибка записи";
    }
}

void  MainWindow::BitMapCreate( QFile  *file )
{
    RGBQUAD color;

//    memset( &color, 0, sizeof(color) );

    // размер картинки в пикселях
    // каждая вторая колонка смещена вниз на половину ячейки
    unsigned Width = m_uColumn * m_tCell.w;
    unsigned Height = m_uRow * m_tCell.h + m_tCell.h/2;
//    unsigned Height = m_uRow * m_tCell.h;

    // очищаем file header
    memset (&m_bitmap.bfh, 0, sizeof(m_bitmap.bfh));

    // заполняем file header
    m_bitmap.bfh.bfType = 0x4D42;
    m_bitmap.bfh.bfOffBits = sizeof(m_bitmap.bfh) + sizeof(m_bitmap.bih) + sizeof(m_bitmap.auColorTable);
    m_bitmap.bfh.bfSize = m_bitmap.bfh.bfOffBits + sizeof(color) * Width * Height;
    m_bitmap.bfh.bfReserved1 = 0;
    m_bitmap.bfh.bfReserved2 = 0;

    // очищаем info header
    memset (&m_bitmap.bih, 0, sizeof(m_bitmap.bih));

    // заполняем info header
    m_bitmap.bih.biSize = sizeof(m_bitmap.bih);
    m_bitmap.bih.biWidth = static_cast<LONG>(Width);
    m_bitmap.bih.biHeight = static_cast<LONG>(Height);
    m_bitmap.bih.biPlanes = 1;
    m_bitmap.bih.biBitCount = 32;
    m_bitmap.bih.biCompression = 0;
    m_bitmap.bih.biSizeImage = 0;
    m_bitmap.bih.biXPelsPerMeter = 0;
    m_bitmap.bih.biYPelsPerMeter = 0;
    m_bitmap.bih.biClrUsed = 0;
    m_bitmap.bih.biClrImportant = 0;

    // очищаем color table
    memset (&m_bitmap.auColorTable, 0, sizeof(m_bitmap.auColorTable));

    // формируем саму картинку
    BitMapFill( file );
}

//------------------------------------------------------------------------------

void  MainWindow::fileFillNormal( QFile  *file )
{
    RGBQUAD      color;
    QDataStream  dstrm( file );
    int  div_i_end, div_i_begin;
    int  div_j_end, div_j_begin;


    dstrm.setVersion(QDataStream::Qt_5_5);

    // пишем заголовок картинки в файл
    dstrm.writeRawData( reinterpret_cast<char*>(&m_bitmap), sizeof(BitMap) );

    // картинка заполняется слева направо снизу вверх.

    // проход по высоте (по строкам)
    for( int i = 0; i < m_bitmap.bih.biHeight; i++ )
    {
        // признак первого пикселя ячейки по высоте
        div_i_begin = i % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_end = (i+1) % static_cast<int>(m_tCell.h);

        // проход по ширине (по столбцам)
        for( int j = 0; j < m_bitmap.bih.biWidth; j++ )
        {
            // признак первого пикселя ячейки по ширине
            div_j_begin = j % static_cast<int>(m_tCell.w);
            // признак последнего пикселя ячейки по ширине
            div_j_end = (j+1) % static_cast<int>(m_tCell.w);

            // белый
            color.rgbRed = 0xFF;
            color.rgbGreen = 0xFF;
            color.rgbBlue = 0xFF;
            color.rgbReserved = 0x0;

            // рисуем столбцы
            if( ( 0 == i ) || ( 0 == div_i_begin ) || ( 0 == div_i_end ) ||
                ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
            {
                // серый
                color.rgbRed = 0x7f;
                color.rgbGreen = 0x7f;
                color.rgbBlue = 0x7f;
                color.rgbReserved = 0x0;
            }

            // пишем пиксель в файл
            dstrm.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }

    if( dstrm.status() != QDataStream::Ok )
    {
        qDebug() << "Ошибка записи";
    }
}

void  MainWindow::fileFillShift( QFile  *file )
{
    RGBQUAD color;
    int div_i_end, div_i_begin = 0;
    int div_i_odd_end, div_i_odd_begin = 0;
    int div_j_end, div_j_begin = 0;
    int row = 1, row_odd = 1, column = 1;
    QDataStream dstrm(file);

    dstrm.setVersion(QDataStream::Qt_5_5);

    // пишем заголовок картинки в файл
    dstrm.writeRawData( reinterpret_cast<char*>(&m_bitmap), sizeof(BitMap) );

    int i, j;

    // картинка заполняется слева направо снизу вверх.

    // проход по высоте (по строкам)
    for(i = 0; i < m_bitmap.bih.biHeight; i++)
    {
        // четные столбцы
        // признак первого пикселя ячейки по высоте
        div_i_begin = i % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_end = (i+1) % static_cast<int>(m_tCell.h);

        // нечетные столбцы
        // признак первого пикселя ячейки по высоте
        div_i_odd_begin = (i + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);
        // признак последнего пикселя ячейки по высоте
        div_i_odd_end = (i+1 + static_cast<int>(m_tCell.h/2)) % static_cast<int>(m_tCell.h);

        // номер текущей строки (для четных столбцов)
        if( 0 == i )
            row = 1;
        else if( 0 == div_i_begin )
            row += 1;

        // номер текущей строки (для нечетных столбцов)
        if( 0 == i )
            row_odd = 1;
        else if( 0 == div_i_odd_begin )
            row_odd += 1;

        // проход по ширине (по столбцам)
        for(j = 0; j < m_bitmap.bih.biWidth; j++)
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

            // белый
            color.rgbRed = 0xFF;
            color.rgbGreen = 0xFF;
            color.rgbBlue = 0xFF;
            color.rgbReserved = 0x0;

            // рисуем четные столбцы
            if( row <= static_cast<int>(m_uRow) )
            {
                if( (column-1)%2 )
                {
                    if( ( 0 == i ) || ( 0 == div_i_begin ) || ( 0 == div_i_end ) ||
                        ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // серый
                        color.rgbRed = 0x7f;
                        color.rgbGreen = 0x7f;
                        color.rgbBlue = 0x7f;
                        color.rgbReserved = 0x0;
                    }
                }
            }

            // рисуем нечетные столбцы
            if( 1 != row_odd )
            {
                if( (column)%2 )
                {
                    if( ( 0 == i ) || ( 0 == div_i_odd_begin ) || ( 0 == div_i_odd_end ) ||
                        ( 0 == j ) || ( 0 == div_j_begin ) || ( 0 == div_j_end ) )
                    {
                        // серый
                        color.rgbRed = 0x7f;
                        color.rgbGreen = 0x7f;
                        color.rgbBlue = 0x7f;
                        color.rgbReserved = 0x0;
                    }
                }
            }

//            qDebug() << "h" << i << "w" << j << "xxx" << j+1 << div_j_begin << div_j_end;
//            qDebug() << "h" << i << "w" << j << "xxx" << row << column;

            // пишем пиксель в файл
            dstrm.writeRawData( reinterpret_cast<char*>(&color), sizeof(color) );
        }
    }

    if( dstrm.status() != QDataStream::Ok )
    {
        qDebug() << "Ошибка записи";
    }
}

void  MainWindow::fileCreate( QFile  *file )
{
    RGBQUAD   color;
    unsigned  uWidth = 0;
    unsigned  uHeight = 0;

    // размер картинки в пикселях
    if( keGridTypeShift == m_uGridType )
    {
        uWidth = m_uColumn * m_tCell.w;
        //uHeight = static_cast<unsigned>( m_uRow / 2.0 * m_tCell.h + m_tCell.h / 2.0 );
        uHeight = m_uRow * m_tCell.h + m_tCell.h / 2;
    }
    else
    {
        uWidth = m_uColumn * m_tCell.w;
        uHeight = m_uRow * m_tCell.h;
    }

    // очищаем file header
    memset (&m_bitmap.bfh, 0, sizeof(m_bitmap.bfh));

    // заполняем file header
    m_bitmap.bfh.bfType = 0x4D42;
    m_bitmap.bfh.bfOffBits = sizeof(m_bitmap.bfh) + sizeof(m_bitmap.bih) + sizeof(m_bitmap.auColorTable);
    m_bitmap.bfh.bfSize = m_bitmap.bfh.bfOffBits + sizeof(color) * uWidth * uHeight;
    m_bitmap.bfh.bfReserved1 = 0;
    m_bitmap.bfh.bfReserved2 = 0;

    // очищаем info header
    memset (&m_bitmap.bih, 0, sizeof(m_bitmap.bih));

    // заполняем info header
    m_bitmap.bih.biSize = sizeof(m_bitmap.bih);
    m_bitmap.bih.biWidth = static_cast<LONG>(uWidth);
    m_bitmap.bih.biHeight = static_cast<LONG>(uHeight);
    m_bitmap.bih.biPlanes = 1;
    m_bitmap.bih.biBitCount = 32;
    m_bitmap.bih.biCompression = 0;
    m_bitmap.bih.biSizeImage = 0;
    m_bitmap.bih.biXPelsPerMeter = 0;
    m_bitmap.bih.biYPelsPerMeter = 0;
    m_bitmap.bih.biClrUsed = 0;
    m_bitmap.bih.biClrImportant = 0;

    // очищаем color table
    memset (&m_bitmap.auColorTable, 0, sizeof(m_bitmap.auColorTable));

    // формируем саму картинку
    if( keGridTypeShift == m_uGridType )
    {
        fileFillShift( file );
    }
    else
    {
        fileFillNormal( file );
    }
}

//------------------------------------------------------------------------------

void  MainWindow::onButtonHandler()
{
    // собственно файл
    QFile file;

    // берем значения с формы
    m_uRow = static_cast<unsigned>(ui->spinRow->value());
    m_uColumn = static_cast<unsigned>(ui->spinColumn->value());

    // формируем имя файла по умолчанию
    QString deffilename = QString( "/pattern%1x%2.bmp" ).arg(m_uRow).arg(m_uColumn);

    // каталог где мы находимся
    QDir *pDir = new QDir( QDir::currentPath() + deffilename );

    // строка с именем каталога где мы находимся
    QString dir( pDir->path() );

    // формиреум путь и имя файла через диалог
    QString filename = QFileDialog::getSaveFileName( this, "Сохранить файл", dir, "Изображение (*.bmp)" );

//    this->setCursor(Qt::WaitCursor);

    QApplication::processEvents();

    if( filename != "" )
    {
        file.setFileName( filename );

        if( !file.open(QIODevice::WriteOnly) )
        {
            qDebug() << "cannot open file" << filename;

            return;
        }
        else
        {
            //        qDebug() << "write to file" << filename;

            file.resize(0);

            fileCreate( &file );

            file.close();

            //        qDebug() << "success";
        }
    }
    else
    {
        // долбоящер не ввел имя файла

        qDebug() << "no filename";
    }

//    this->setCursor(Qt::ArrowCursor);
}

void  MainWindow::onInfoHandler()
{
    Info *pInfo = new Info;

    pInfo->setWindowTitle( "О программе" );
    pInfo->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );
    pInfo->setWindowFlags( Qt::WindowSystemMenuHint );
    pInfo->setFixedSize( 320, 280 );

    pInfo->exec();
}

void  MainWindow::onManHandler()
{
    Man *pMan = new Man;

    pMan->setWindowTitle( "Руководство" );
    pMan->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );
    pMan->setWindowFlags( Qt::WindowSystemMenuHint );
    pMan->setFixedSize( 320, 280 );

    pMan->exec();
}

void  MainWindow::onChangeItem( int  index )
{
    if( 0 == index )
        m_uItemType = keItemTypeRectan;
    else if( 1 == index )
        m_uItemType = keItemTypeSquare;

    setCellSize();
}

void  MainWindow::onChangeGrid( int  index )
{
    if( 0 == index )
        m_uGridType = keGridTypeNormal;
    else if( 1 == index )
        m_uGridType = keGridTypeShift;
}

//------------------------------------------------------------------------------

void  MainWindow::setCellSize()
{
    // размер ячейки в пикселях

    if( keItemTypeRectan == m_uItemType )
    {
        //qDebug() << "rectangle";

        m_tCell.h = 20;
        m_tCell.w = 10;
    }
    else if( keItemTypeSquare == m_uItemType )
    {
        //qDebug() << "square";

        m_tCell.h = 10;
        m_tCell.w = 10;
    }
    else
    {
        //qDebug() << "item not specified!";

        m_tCell.h = 1;
        m_tCell.w = 1;
    }
}

//------------------------------------------------------------------------------
