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
    TNewDialog(QWidget *parent = nullptr);
    ~TNewDialog() override;

signals:
    void       dlgCreate();

private Q_SLOTS:
    void       onChangeItem(int  index);
    void       onChangeGrid(int  index);

    void       onChangeRow(int value);
    void       onChangeColumn(int value);

    void 	   onCreate(QAbstractButton *btn);
    void 	   onReset(QAbstractButton *btn);
    void 	   onReject(QAbstractButton *btn);

private:
    std::unique_ptr<TPrivNewDialog> priv__;

    void       do_create();
    void       do_reset();

    void       closeEvent(QCloseEvent *event) override;
};

//------------------------------------------------------------------------------

#endif // NEWDIALOG_H
