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
    m_nRow = 1;
    m_nColumn = 1;

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

void MainWindow::ButtonHandler()
{
    // инкремент счетчика нажатий
    m_nCount += 1;

    // берем значения с формы
    m_nRow = ui->spinRow->value();
    m_nColumn = ui->spinColumn->value();

    // лог
    qDebug() << "count" << m_nCount << "row" << m_nRow << "column" << m_nColumn;
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
