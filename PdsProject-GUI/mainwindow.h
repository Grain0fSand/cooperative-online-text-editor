#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <chrono>

class Killable_taks {
    std::atomic<bool> alive;
    int milliseconds;
    std::atomic<bool> launched;
    std::atomic<bool> is_lambda_setted;
    std::function<void()> lambda;

public:

    Killable_taks(int milliseconds) : alive(true),milliseconds(milliseconds),is_lambda_setted(false) {
        if (milliseconds < 10){
            throw "timeout too short, maybe it is an error";
        }
    }

    void start(std::function<void()> func){
        lambda = [func,this]{
            while(alive){
                std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
                func();
            }
        };

        is_lambda_setted = true;
        launched = true;

        std::thread(lambda).detach();
    }

    void stop(){
        if (!is_lambda_setted || !launched)
            throw "cannot stop a thread never started or created";
        this->alive = false;
        this->launched = false;
    }

    void restart(){
        if (!is_lambda_setted)
            throw "cannot restart a task never started";
        if (launched)
            throw "cannot restart a thread not stopped";

        this->alive = true;
        this->launched = true;

        std::thread(lambda).detach();
    }
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


private:
    Ui::MainWindow *ui;
    Killable_taks background_task;
};

#endif // MAINWINDOW_H
