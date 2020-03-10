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

    m_pDir = new QDir(QDir::currentPath()+"/field");

    // заголовок формы
    this->setWindowTitle("PatternDraw");

    // иконка формы
    this->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );

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
    color.rgbReserved = 0;

    QDataStream dstrm(file);
    dstrm.setVersion(QDataStream::Qt_5_5);
    dstrm.writeRawData((char*)&m_bitmap,sizeof(m_bitmap));

    int i, j;

    for(i = 0; i < m_bitmap.bih.biHeight; i++)
    {
        for(j = 0; j < m_bitmap.bih.biWidth; j++)
        {
            color.rgbRed = 0x7F;
            color.rgbGreen = 0x7F;
            color.rgbBlue = 0x7F;

//            dstrm.writeRawData((char*)&color,sizeof(color));
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

    memset( &color, 0, sizeof(color) );

    // размер картинки
    unsigned Width = m_uColumn;
    unsigned Height = m_uRow;

    // очищаем file header
    memset (&m_bitmap.bfh, 0, sizeof(m_bitmap.bfh));

    // заполняем file header
    m_bitmap.bfh.bfType = 0x4D42;
    m_bitmap.bfh.bfOffBits = sizeof(m_bitmap.bfh) + sizeof(m_bitmap.bih);
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
    qDebug() << "count" << m_nCount << "row" << m_uRow << "column" << m_uColumn;

    QString dir(m_pDir->path());

//    QString fileName =
//            QFileDialog::getSaveFileName(0, tr("Сохранить поле"),
//                          dir,tr("(*.bmp)") );

    QFile file;

    file.setFileName( "test-bmp.bmp" );

    if( !file.open(QIODevice::WriteOnly) )
    {
        qDebug() << "cannot open file";

        return;
    }
    else
    {
        qDebug() << "write to file";

        file.resize(0);

        BitMapCreate( &file );

        file.close();
    }
}

void MainWindow::InfoHandler()
{
    Info *pInfo = new Info;

    pInfo->setWindowTitle( "О программе" );
    pInfo->setWindowIcon( QIcon( ":/PatternDraw.ico" ) );
    pInfo->setWindowFlags( Qt::WindowSystemMenuHint );
    pInfo->setFixedSize( 320, 280 );

    pInfo->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
