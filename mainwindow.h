#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;

    int  m_nCount;
    int  m_nRow;
    int  m_nColumn;
};

#endif // MAINWINDOW_H
