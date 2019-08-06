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

    void keyPressed();
    void textCopied();
    void textPasted();
    void collaborativeURI_Copied();
    void collaborativeURI_Pasted();

public slots:
    void exportPDF();
    void selectFont();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
