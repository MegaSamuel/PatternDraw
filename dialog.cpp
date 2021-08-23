#include "dialog.h"


Info::Info()
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    QLabel *caption = new QLabel;
    caption->setText( "Программа PatternDraw, версия 0.1.3" );
    caption->setStyleSheet("font-weight: bold");

    QPlainTextEdit *ptInfo = new QPlainTextEdit;
    ptInfo->setReadOnly(true);
    ptInfo->setFrameStyle(QFrame::NoFrame);
    ptInfo->setStyleSheet("background-color: transparent");

//    ptInfo->appendPlainText("Программа PatternDraw, версия 0.1.3");
//    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Разработчик Медведев С.А.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Программа поставляется по принципу \"AS IS\" (\"как есть\"). Никакие гарантии не прилагаются и не предусматриваются. Вы используете данную программу на свой страх и риск. Разработчик программы не будет отвечать ни за какие потери или искажения данных, нарушения работоспособности других программ и системы, а также за любую упущенную выгоду в процессе использования или неправильного использования данной программы.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("По вопросам писать на cepaleme@gmail.com");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("2021 г.");

    QLabel *label = new QLabel( "" );

    // читаем картинку
    QPixmap myPixmap( ":/PatternDraw.ico" );

    if( !myPixmap.isNull() )
    {
        //qDebug() << "w" << myPixmap.size().width() << "h" << myPixmap.size().height();

        // растягиваем лэйбл до размеров картинки
        label->setMinimumWidth( myPixmap.size().width() );
        label->setMinimumHeight( myPixmap.size().height() );

        // ставим картинку в лэйбл
        label->setPixmap( myPixmap );
    }

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    hlayout1->addWidget(label);
    hlayout1->addWidget(ptInfo);

    QPushButton *quitButton = new QPushButton("Закрыть");

    connect( quitButton, SIGNAL(clicked()), SLOT(accept()));
    //connect( quitButton, &QPushButton::clicked, &QDialog::accept );

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QSpacerItem *item = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hlayout2->addSpacerItem(item);
    hlayout2->addWidget(quitButton);

    vlayout->addWidget(caption);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);

    setLayout(vlayout);
}

Info::~Info()
{

}

Man::Man()
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );

    QLabel *caption = new QLabel;
    caption->setText( "Руководство пользователя программы PatternDraw" );
    caption->setStyleSheet("font-weight: bold");

    QPlainTextEdit *ptInfo = new QPlainTextEdit;
    ptInfo->setReadOnly(true);
    ptInfo->setFrameStyle(QFrame::NoFrame);
    ptInfo->setStyleSheet("background-color: transparent");

//    ptInfo->appendPlainText("Руководство пользователя программы PatternDraw.");
//    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Программа предназначена для создания сетки схемы вязания и ее сохранения в виде растрового изображения в формате PNG.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Для создания сетки необходимо ввести количество строк и количество столбцов и нажать кнопку \"Сохранить...\". В открывшемся диалоговом окне выбрать место и имя файла для сохранения, нажать кнопку \"Сохранить\". В результате этих действий будет создан файл содержащий сетку схемы.");
    ptInfo->appendPlainText("");

    QLabel *label = new QLabel( "" );

    // читаем картинку
    QPixmap myPixmap( ":/PatternDraw.ico" );

    if( !myPixmap.isNull() )
    {
        //qDebug() << "w" << myPixmap.size().width() << "h" << myPixmap.size().height();

        // растягиваем лэйбл до размеров картинки
        label->setMinimumWidth( myPixmap.size().width() );
        label->setMinimumHeight( myPixmap.size().height() );

        // ставим картинку в лэйбл
        label->setPixmap( myPixmap );
    }

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    hlayout1->addWidget(label);
    hlayout1->addWidget(ptInfo);

    QPushButton *quitButton = new QPushButton("Закрыть");

    connect(quitButton, SIGNAL(clicked()), SLOT(accept()));

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    QSpacerItem *item = new QSpacerItem( 1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hlayout2->addSpacerItem(item);
    hlayout2->addWidget(quitButton);

    vlayout->addWidget(caption);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);

    setLayout(vlayout);
}

Man::~Man()
{

}
