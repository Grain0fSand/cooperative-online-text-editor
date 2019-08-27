#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QKeyEvent>
#include <thread>
#include <chrono>
#include <qthread.h>

class Periodic_task : public QThread
{
    Q_OBJECT

    int milliseconds;
    std::atomic<bool> running;

public:

    void run(void) override {
        while (running){
            std::this_thread::sleep_for(std::chrono::milliseconds(this->milliseconds));
            emit tick_clock();
        }
    }

    void cancel(){
        this->running = false;
    }

    Periodic_task(int milliseconds) : milliseconds(milliseconds),running(true) {
        if (milliseconds < 10){
            throw "timeout too short, maybe it is an error";
        }
    }

signals:
    void tick_clock();

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void collaborativeURI_Copied();
    void collaborativeURI_Pasted();
    bool eventFilter(QObject *obj,QEvent* event);

public slots:
    void exportPDF();
    void selectFont(int);
    void selectSize(int);
    void redrawBlinkingImage();
    void makeBold();
    void makeItalic();
    void makeUnderline();
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();
    void memorizeSelection();
    void checkTextProperty();
    void openSettings();

protected slots:
    void textChanged();

signals:
    void setComboSize(int);
    void setComboFont(QFont);

private:
    Ui::MainWindow *ui;
    Periodic_task background_task;
    int last_cursor_position;
    int toDelete;
    QStringList fontSizes;
    QShortcut *shortcutUndo;
    QShortcut *shortcutRedo;
    QClipboard *clipboard;
};

#endif // MAINWINDOW_H
