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

    TGridData         m_tDefData;
    TGridData         m_tCurData;

    inline TPrivNewDialog() {
        m_ptComboItem = Q_NULLPTR;
        m_ptComboGrid = Q_NULLPTR;

        m_ptSpinRow = Q_NULLPTR;
        m_ptSpinColumn = Q_NULLPTR;

        m_ptBtnBox = Q_NULLPTR;

        m_ptGridLayout = new QGridLayout;
    }

    ~TPrivNewDialog() {
        m_ptGridLayout->deleteLater();
    }
};

//------------------------------------------------------------------------------

TNewDialog::TNewDialog(QWidget *parent) : QDialog(parent) {
    int row = 0;

    priv__ = std::unique_ptr<TPrivNewDialog>(new TPrivNewDialog);

    QLabel *title;

    QComboBox *combo;

    // ячейка
    combo = new QComboBox(this);
    std::vector<QString> vctComboItems = {tr("Прямоугольник"), tr("Квадрат")};
    for(const QString& it : vctComboItems)
        combo->addItem(it);
    combo->setCurrentIndex(priv__->m_tDefData.nItemType);
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TNewDialog::onChangeItem);

    priv__->m_ptComboItem = combo;
    title = new QLabel(tr("Ячейка") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(combo, row, 1, 1, 1);
    row++;

    // смещение
    combo = new QComboBox(this);
    std::vector<QString> vctComboGrid = {tr("Без смещения"), tr("Со смещением")};
    for(const QString& it : vctComboGrid)
        combo->addItem(it);
    combo->setCurrentIndex(priv__->m_tDefData.nGridType);
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TNewDialog::onChangeGrid);

    priv__->m_ptComboGrid = combo;
    title = new QLabel(tr("Смещение") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(combo, row, 1, 1, 1);
    row++;

    QSpinBox *spin;

    // ряды
    spin = new QSpinBox(this);
    spin->setMinimum(1);
    spin->setMaximum(128);
    spin->setValue(priv__->m_tDefData.nRow);
    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &TNewDialog::onChangeRow);

    priv__->m_ptSpinRow = spin;
    title = new QLabel(tr("Количество рядов") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(spin, row, 1, 1, 1);
    row++;

    // петли
    spin = new QSpinBox(this);
    spin->setMinimum(1);
    spin->setMaximum(128);
    spin->setValue(priv__->m_tDefData.nColumn);
    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &TNewDialog::onChangeColumn);

    priv__->m_ptSpinColumn = spin;
    title = new QLabel(tr("Количество петель") + ":", this);
    priv__->m_ptGridLayout->addWidget(title, row, 0, 1, 1);
    priv__->m_ptGridLayout->addWidget(spin, row, 1, 1, 1);
    row++;

    // пустота
    title = new QLabel(tr(""), this);
    priv__->m_ptGridLayout->addWidget( title, row, 0, 1, 1 );
    row++;
    row = priv__->m_ptGridLayout->rowCount();

    // создаем диалоговые кнопки
    auto  box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Reset | QDialogButtonBox::Cancel, this);
    connect(box, &QDialogButtonBox::clicked, this, &TNewDialog::onCreate);
    connect(box, &QDialogButtonBox::clicked, this, &TNewDialog::onReset);
    connect(box, &QDialogButtonBox::clicked, this, &TNewDialog::onReject);

    // принудительно переименовываем кнопку Ok
    QPushButton *button = box->button(QDialogButtonBox::Ok);
    button->setText(tr("Ок"));
    button->setDefault(true);

    // принудительно переименовываем кнопку Reset
    button = box->button(QDialogButtonBox::Reset);
    button->setText(tr("По умолчанию"));

    // принудительно переименовываем кнопку Cancel
    button = box->button(QDialogButtonBox::Cancel);
    button->setText(tr("Отмена"));

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->m_ptGridLayout->addWidget(box, row, 0, 1, 2);
    row++;

    priv__->m_ptGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(priv__->m_ptGridLayout);

    setWindowTitle(tr("Создание сетки"));

    // убираем знак вопроса из заголовка
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

TNewDialog::~TNewDialog() {
}

//------------------------------------------------------------------------------

// тип ячейки
void TNewDialog::onChangeItem(int  index) {
    int type = keItemTypeRectan;

    if(0 == index)
        type = keItemTypeRectan;
    else if(1 == index)
        type = keItemTypeSquare;

    priv__->m_tCurData.nItemType = type;
}

// смещение
void TNewDialog::onChangeGrid(int  index) {
    int type = keGridTypeShift;

    if(0 == index)
        type = keGridTypeNormal;
    else if( 1 == index )
        type = keGridTypeShift;

    priv__->m_tCurData.nGridType = type;
}

// количество рядов
void TNewDialog::onChangeRow(int value) {
    priv__->m_tCurData.nRow = value;
}

// количество петель
void TNewDialog::onChangeColumn(int value) {
    priv__->m_tCurData.nColumn = value;
}

// обработка нажатия Создать
void TNewDialog::onCreate(QAbstractButton *btn) {
    if(QDialogButtonBox::AcceptRole != priv__->m_ptBtnBox->buttonRole(btn))
        return;

    do_create();

    Q_EMIT(dlgCreate());

    close();
}

// обработка нажатия Сброс
void TNewDialog::onReset(QAbstractButton*  btn) {
    if(QDialogButtonBox::ResetRole != priv__->m_ptBtnBox->buttonRole(btn))
        return;

    do_reset();
}

// обработка нажатия Отмена
void TNewDialog::onReject(QAbstractButton*  btn) {
    if(QDialogButtonBox::RejectRole != priv__->m_ptBtnBox->buttonRole(btn))
        return;

    do_reset();

    close();
}

//------------------------------------------------------------------------------

void TNewDialog::do_create() {
    glb().tGridData = priv__->m_tCurData;
}

void TNewDialog::do_reset() {
    priv__->m_tCurData = priv__->m_tDefData;

    priv__->m_ptComboItem->setCurrentIndex(priv__->m_tDefData.nItemType);
    priv__->m_ptComboGrid->setCurrentIndex(priv__->m_tDefData.nGridType);

    priv__->m_ptSpinRow->setValue(priv__->m_tDefData.nRow);
    priv__->m_ptSpinColumn->setValue(priv__->m_tDefData.nColumn);
}

//------------------------------------------------------------------------------

void TNewDialog::closeEvent(QCloseEvent *event) {
    do_reset();
    event->accept();
}

//------------------------------------------------------------------------------
