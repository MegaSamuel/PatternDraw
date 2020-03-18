#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <bitmap.h>
#include <QDir>
#include <QFile>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void  ButtonHandler();
    void  InfoHandler();
    void  ManHandler();

private:
    Ui::MainWindow *ui;

    void  BitMapCreate(QFile *file);
    void  BitMapFill(QFile *file);

    BitMap m_bitmap;

    TCell  m_tCell;

    unsigned  m_uRow;
    unsigned  m_uColumn;
};

#endif // MAINWINDOW_H
