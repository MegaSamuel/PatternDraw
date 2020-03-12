#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // дефолтные значения переменных
    m_nCount = 0;
    m_uRow = 1;
    m_uColumn = 1;

    // размер ячейки в пикселях
    m_tCell.h = 20;
    m_tCell.w = 10;

    m_pDir = new QDir(QDir::currentPath()+"/field");

    // заголовок формы
    this->setWindowTitle("PatternDraw");

    // иконка формы
    this->setWindowIcon( QIcon( ":/PatternDraw.png" ) );

    // прячем избыточные кнопки заголовка формы
    this->setWindowFlag(Qt::WindowMinMaxButtonsHint,false);
    this->setWindowFlag(Qt::WindowSystemMenuHint,false);

    // ловим нажатие кнопки
    connect( ui->pushButton, SIGNAL(pressed()), this, SLOT(ButtonHandler()) );

    // вызов справки о программе
    connect( ui->actioninfo, SIGNAL(triggered()), SLOT(InfoHandler()) );
}

void MainWindow::BitMapFill(QFile *file)
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

void MainWindow::BitMapCreate(QFile *file)
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

void MainWindow::ButtonHandler()
{
    // инкремент счетчика нажатий
    m_nCount += 1;

    // берем значения с формы
    m_uRow = static_cast<unsigned>(ui->spinRow->value());
    m_uColumn = static_cast<unsigned>(ui->spinColumn->value());

    // лог
//    qDebug() << "count" << m_nCount << "row" << m_uRow << "column" << m_uColumn;

    QString dir(m_pDir->path());

//    QString fileName =
//            QFileDialog::getSaveFileName(0, tr("Сохранить поле"),
//                          dir,tr("(*.bmp)") );

    QFile file;
    QString filename;

    // формируем имя файла
    filename = QString( "pattern%1x%2.bmp" ).arg(m_uRow).arg(m_uColumn);

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

        BitMapCreate( &file );

        file.close();

//        qDebug() << "success";
    }
}

void MainWindow::InfoHandler()
{
    Info *pInfo = new Info;

    pInfo->setWindowTitle( "О программе" );
    pInfo->setWindowIcon( QIcon( ":/PatternDraw.png" ) );
    pInfo->setWindowFlags( Qt::WindowSystemMenuHint );
    pInfo->setFixedSize( 320, 280 );

    pInfo->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
