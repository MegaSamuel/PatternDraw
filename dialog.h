#ifndef DIALOG
#define DIALOG

#include <QDialog>
#include <QtWidgets>


class Info:public QDialog
{
    Q_OBJECT

public:
    explicit Info();
    ~Info();
};

class Man:public QDialog
{
    Q_OBJECT

public:
    explicit Man();
    ~Man();
};

#endif // DIALOG

