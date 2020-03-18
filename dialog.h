#ifndef DIALOG
#define DIALOG

#include <QDialog>
#include <QtWidgets>


class Info:public QDialog
{
    Q_OBJECT
public:
    Info();
    ~Info();
};

class Man:public QDialog
{
    Q_OBJECT
public:
    Man();
    ~Man();
};

#endif // DIALOG

