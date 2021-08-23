#ifndef DIALOG
#define DIALOG

#include <QDialog>
#include <QtWidgets>

//------------------------------------------------------------------------------

class Info : public QDialog
{
    Q_OBJECT

public:
    explicit Info( QWidget *parent = nullptr );
    ~Info();
};

//------------------------------------------------------------------------------

class Man : public QDialog
{
    Q_OBJECT

public:
    explicit Man( QWidget *parent = nullptr );
    ~Man();
};

//------------------------------------------------------------------------------

#endif // DIALOG
