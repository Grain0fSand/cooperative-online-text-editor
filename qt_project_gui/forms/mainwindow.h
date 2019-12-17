#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QShortcut>
#include <QKeyEvent>
#include <thread>
#include <chrono>
#include <qthread.h>
#include <atomic>
#include <stack>
#include "usertag.h"
#include "web_requests/online_query.h"
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
    std::deque<QEvent::Type> lastEventType;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //MainWindow(const MainWindow&) = delete;
    //MainWindow& operator=(const MainWindow&) = delete;

    bool eventFilter(QObject *obj,QEvent* event);
    void closeEvent ( QCloseEvent * event );
    void sendInvitationEmail(QString docName, QString destEmailAddress);
    void setupFontComboBoxes(QComboBox *comboSize, QComboBox *comboFamily);
    void setupStatusBar();

public slots:
    void update_id(std::string id);
    void exitFromEditor();
    void backToLogin();
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
    void changeEditorStatus();
    void populateUserTagList();
    void arrangeUserTagList(std::vector<exchangeable_data::user>);

protected slots:
    void textChanged(int, int, int);

signals:
    void stopQueryLoop();
    void setComboSize(int);
    void setComboFont(int);

private slots:
    void on_onlineRollButton_clicked();
    void on_offlineRollButton_clicked();
    void update_online_users_and_cursors_positions(std::vector<exchangeable_data::user> vector);

private:

    void addUserTag(QString username, bool status, QPixmap avatar, QColor color);
    Ui::MainWindow *ui;
    Periodic_task background_task;
    OnlineQuery* query; // for online updates of crdt
};

#endif // MAINWINDOW_H