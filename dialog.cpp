#include "dialog.h"


Info::Info()
{
    QPalette pal;

    QVBoxLayout *pvblInfo = new QVBoxLayout;
    pvblInfo->setSpacing(10);
    pvblInfo->setMargin(5);

    QPlainTextEdit *ptInfo = new QPlainTextEdit;
    ptInfo->setReadOnly(true);
    pal.setColor(ptInfo->backgroundRole(), Qt::gray);
    ptInfo->setPalette(pal);
    ptInfo->setAutoFillBackground(true);

    ptInfo->appendPlainText("Программа PatternDraw, версия 0.1.2");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Разработчик Медведев С.А.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Программа поставляется по принципу \"AS IS\" (\"как есть\"). Никакие гарантии не прилагаются и не предусматриваются. Вы используете данную программу на свой страх и риск. Разработчик программы не будет отвечать ни за какие потери или искажения данных, нарушения работоспособности других программ и системы, а также за любую упущенную выгоду в процессе использования или неправильного использования данной программы.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("По вопросам писать на cepaleme@gmail.com");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("2020 г.");

    QPushButton *quitButton = new QPushButton("OK");

    connect(quitButton, SIGNAL(clicked()),SLOT(accept()));

    pvblInfo->addWidget(ptInfo);
    pvblInfo->addWidget(quitButton);

    setLayout(pvblInfo);
}

Info::~Info()
{

}

Man::Man()
{
    QPalette pal;

    QVBoxLayout *pvblInfo = new QVBoxLayout;
    pvblInfo->setSpacing(10);
    pvblInfo->setMargin(5);

    QPlainTextEdit *ptInfo = new QPlainTextEdit;
    ptInfo->setReadOnly(true);
    pal.setColor(ptInfo->backgroundRole(), Qt::gray);
    ptInfo->setPalette(pal);
    ptInfo->setAutoFillBackground(true);

    ptInfo->appendPlainText("Руководство пользователя программы PatternDraw.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Программа предназначена для создания сетки схемы вязания и ее сохранения в виде растрового изображения в формате BMP.");
    ptInfo->appendPlainText("");
    ptInfo->appendPlainText("Для создания сетки необходимо ввести количество строк и количество столбцов и нажать кнопку \"Сохранить...\". В открывшемся диалоговом окне выбрать место и имя файла для сохранения, нажать кнопку \"Сохранить\". В результате этих действий будет создан файл содержащий сетку схемы.");
    ptInfo->appendPlainText("");

    QPushButton *quitButton = new QPushButton("OK");

    connect(quitButton, SIGNAL(clicked()),SLOT(accept()));

    pvblInfo->addWidget(ptInfo);
    pvblInfo->addWidget(quitButton);

    setLayout(pvblInfo);
}

Man::~Man()
{

}
