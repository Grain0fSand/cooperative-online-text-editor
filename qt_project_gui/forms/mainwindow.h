#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QShortcut>
#include <QKeyEvent>
#include <thread>
#include <chrono>
#include <qthread.h>
#include "usertag.h"
#include "../data_structure/crdt.h"
#include "../data_structure/session_data.h"

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
    SessionData sessionData;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    static MainWindow& getInstance(){
        static MainWindow instance;
        token="1";
        return instance;
    }

    void collaborativeURI_Copied();
    void collaborativeURI_Pasted();
    bool eventFilter(QObject *obj,QEvent* event);
    void sendInvitationEmail(QString destEmailAddress);
    void setupFontComboBoxes(QComboBox *comboSize, QComboBox *comboFamily);
    void setupStatusBar();

public slots:
    void exportPDF();
    void selectFont(int);
    void selectSize(int);
    void makeBold();
    void makeItalic();
    void makeUnderline();
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();
    void checkTextProperty();
    void insertRemoteCursor();
    void reqInvitationEmailAddress();
    void disableEditor();
    void newUserTag();

protected slots:
    void textChanged(int, int, int);

signals:
    void setComboSize(int);
    void setComboFont(int);

private slots:
    void on_onlineRollButton_clicked();

    void on_offlineRollButton_clicked();

    void on_actionTestActions_triggered();

private:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addUserTag(QString username, bool status, QPixmap avatar, QColor color);
    Ui::MainWindow *ui;
    Periodic_task background_task;
    QClipboard *clipboard;
    std::list<UserTag*> usersList;
};

#endif // MAINWINDOW_H
