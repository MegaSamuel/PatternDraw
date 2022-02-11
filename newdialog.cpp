#include "global.h"
#include "newdialog.h"

//------------------------------------------------------------------------------

class TPrivNewDialog
{
    friend class TNewDialog;

public:
    QComboBox        *m_ptComboItem;
    QComboBox        *m_ptComboGrid;

    QSpinBox         *m_ptSpinRow;
    QSpinBox         *m_ptSpinColumn;

    QDialogButtonBox *m_ptBtnBox;

    QGridLayout      *m_ptGridLayout;

    inline TPrivNewDialog()
    {
        m_ptComboItem = Q_NULLPTR;
        m_ptComboGrid = Q_NULLPTR;

        m_ptSpinRow = Q_NULLPTR;
        m_ptSpinColumn = Q_NULLPTR;

        m_ptBtnBox = Q_NULLPTR;

        m_ptGridLayout = new QGridLayout;
    }

    ~TPrivNewDialog()
    {
        m_ptGridLayout->deleteLater();
    }
};

//------------------------------------------------------------------------------

TNewDialog::TNewDialog(QWidget *parent) : QDialog(parent)
{
    QPushButton 	*btn;
    QLabel          *title;
    int              row = 0;

    priv__ = std::unique_ptr<TPrivNewDialog>(new TPrivNewDialog);

    title = new QLabel( tr("Разработчик") + ":", this );
    auto  pCompanyName = new QLabel( tr("АО «Авионика вертолетов»"), this );
    priv__->m_ptGridLayout->addWidget( title, row, 0, 1, 1 );
    priv__->m_ptGridLayout->addWidget( pCompanyName, row, 1, 1, 1 );
    row++;
    row = priv__->m_ptGridLayout->rowCount();

    QComboBox *combo;

    // ячейка
    combo = new QComboBox(this);
    std::vector<QString> vctComboItems = {"Прямоугольник", "Квадрат"};
    for(const QString& it : vctComboItems)
        combo->addItem(it);
    combo->setCurrentIndex(keItemTypeRectan);
    connect( combo, SIGNAL( currentIndexChanged(int) ), this, SLOT(onComboNetwork(int)) );

    priv__->m_ptComboItem = combo;
    title = new QLabel(tr("Ячейка") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(combo, row, 1, 1, 1);
    row++;

    // смещение
    combo = new QComboBox(this);
    std::vector<QString> vctComboGrid = {"Прямоугольник", "Квадрат"};
    for(const QString& it : vctComboItems)
        combo->addItem(it);
    combo->setCurrentIndex(keItemTypeRectan);
    connect( combo, SIGNAL( currentIndexChanged(int) ), this, SLOT(onComboNetwork(int)) );

    priv__->m_ptComboItem = combo;
    title = new QLabel(tr("Ячейка") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(combo, row, 1, 1, 1);
    row++;

    // ряды

    // петли

    btn = new QPushButton( this );
    setPathName( btn, config );
    connect( btn, &QPushButton::clicked, this, &TNewDialog::onBtnConfigPath );
    priv__->m_ptBtnConfigPath = btn;
    title = new QLabel( tr("Путь к конфигурационным файлам") + ":", this );
    priv__->m_ptGridLayout->addWidget( title, row, 0, 1, 1 );
    priv__->m_ptGridLayout->addWidget( btn, row, 1, 1, 1 );
    row++;

    btn = new QPushButton( this );
    setPathName( btn, binary );
    connect( btn, &QPushButton::clicked, this, &TNewDialog::onBtnBinaryPath );
    priv__->m_ptBtnBinaryPath = btn;
    title = new QLabel( tr("Путь к исполняемым файлам") + ":", this );
    priv__->m_ptGridLayout->addWidget( title, row, 0, 1, 1 );
    priv__->m_ptGridLayout->addWidget( btn, row, 1, 1, 1 );
    row++;

    btn = new QPushButton( this );
    setPathName( btn, logger );
    connect( btn, &QPushButton::clicked, this, &TNewDialog::onBtnLoggerPath );
    priv__->m_ptBtnLoggerPath = btn;
    title = new QLabel( tr("Путь к лог файлам") + ":", this );
    priv__->m_ptGridLayout->addWidget( title, row, 0, 1, 1 );
    priv__->m_ptGridLayout->addWidget( btn, row, 1, 1, 1 );
    row++;

    // set minimum row height for m_ptGridLayout based on maximum height of sizeHint() for all widgets in m_ptGridLayout
    QLayoutItem    *pItem;
    int             nMinRowHeight = 0;
    for( int index = 0; index < priv__->layout->count(); index++ )
    {
        if( Q_NULLPTR != ( pItem = priv__->layout->itemAt( index ) ) )
        {
            if( pItem->sizeHint().height() > nMinRowHeight )
                nMinRowHeight = pItem->sizeHint().height();
        }
    }
    for( int i = 0; i < priv__->layout->rowCount(); i++ )
        priv__->layout->setRowMinimumHeight( i, nMinRowHeight );

    // создаем диалоговые кнопки
    auto  box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Reset, this );
    connect( box, &QDialogButtonBox::accepted, this, &TNewDialog::close );
    connect( box, &QDialogButtonBox::rejected, this, &TNewDialog::close );
    connect( box, &QDialogButtonBox::clicked, this, &TNewDialog::onReset );

    // принудительно переименовываем кнопку Ok
    QPushButton 	*ptr = box->button( QDialogButtonBox::Ok );
    if( ptr )
        ptr->setText( tr( "Сохранить" ) );

    // принудительно переименовываем кнопку Reset
    ptr = box->button( QDialogButtonBox::Reset );
    if( ptr )
        ptr->setText( tr( "Сброс" ) );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->layout->addWidget( box, row, 0, 1, 2 );
    row++;

    priv__->layout->setSizeConstraint( QLayout::SetFixedSize );
    setLayout( priv__->layout );

    setWindowTitle( tr( "Диалог настроек" ) );
}

TNewDialog::~TNewDialog() {

}

//------------------------------------------------------------------------------

// тип ячейки
void TNewDialog::onChangeItem(int  index) {
    unsigned type = keItemTypeRectan;

    if(0 == index)
        type = keItemTypeRectan;
    else if(1 == index)
        type = keItemTypeSquare;

    glb().m_uItemType = type;
}

// смещение
void TNewDialog::onChangeGrid(int  index) {
    unsigned type = keGridTypeShift;

    if(0 == index)
        type = keGridTypeNormal;
    else if( 1 == index )
        type = keGridTypeShift;

    glb().m_uGridType = type;
}

// количество рядов
void TNewDialog::onChangeRow(int value) {
    glb().m_uRow = static_cast<unsigned>(value);
}

// количество петель
void TNewDialog::onChangeColumn(int value) {
    glb().m_uColumn = static_cast<unsigned>(value);
}

// обработка нажатия Сброс
void TNewDialog::onReset(QAbstractButton*  btn)
{
    if(QDialogButtonBox::ResetRole != priv__->m_ptBtnBox->buttonRole(btn))
        return;

    priv__->m_ptComboItem->setCurrentIndex(keItemTypeRectan);
    priv__->m_ptComboGrid->setCurrentIndex(keGridTypeShift);

    priv__->m_ptSpinRow->setValue(DLG_ROW_COUNT);
    priv__->m_ptSpinColumn->setValue(DLG_COLUMN_COUNT);
}

//------------------------------------------------------------------------------
