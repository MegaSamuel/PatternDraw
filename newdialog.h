#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QtGui>
#include <QtWidgets>

//------------------------------------------------------------------------------

class TPrivNewDialog;

//------------------------------------------------------------------------------

class TNewDialog : public QDialog
{
    friend class TPrivNewDialog;
    Q_OBJECT

public :
    inline static constexpr int DLG_ROW_COUNT = 10;
    inline static constexpr int DLG_COLUMN_COUNT = 10;

    TNewDialog(QWidget *parent = nullptr);
    ~TNewDialog();

private Q_SLOTS:
    void       onChangeItem(int  index);
    void       onChangeGrid(int  index);

    void       onChangeRow(int value);
    void       onChangeColumn(int value);

    void 	   onReset(QAbstractButton *btn);

private:
    std::unique_ptr<TPrivNewDialog> priv__;
};

//------------------------------------------------------------------------------

#endif // NEWDIALOG_H
