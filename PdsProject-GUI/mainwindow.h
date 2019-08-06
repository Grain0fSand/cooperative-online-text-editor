#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <chrono>
#include <qthread.h>

class Periodic_task : public QThread
{
    Q_OBJECT

    int milliseconds;
    std::atomic<bool> launched;

public:

    void run(void) override {
        launched = true;
        while (true){
            std::this_thread::sleep_for(std::chrono::milliseconds(this->milliseconds));
            if (launched){
                emit tick_clock();
            }
        }
    }

    Periodic_task(int milliseconds) : milliseconds(milliseconds){
        if (milliseconds < 10){
            throw "timeout too short, maybe it is an error";
        }
    }

    void restart_tick(){
        this->launched = true;
    }

    void stop_tick(){
        this->launched = false;
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

    void keyPressed();
    void textCopied();
    void textPasted();
    void collaborativeURI_Copied();
    void collaborativeURI_Pasted();

public slots:
    void exportPDF();
    void selectFont();
    void redrawBlinkingImage();


private:
    Ui::MainWindow *ui;
    Periodic_task background_task;
};

#endif // MAINWINDOW_H
