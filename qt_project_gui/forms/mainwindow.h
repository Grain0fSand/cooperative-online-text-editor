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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QEvent::Type lastEventType;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *obj,QEvent* event);
    void sendInvitationEmail(QString docName, QString destEmailAddress);
    void setupFontComboBoxes(QComboBox *comboSize, QComboBox *comboFamily);
    void setupStatusBar();

public slots:
    void exitFromEditor();
    void backToPersonalPage();
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

private:

    Ui::MainWindow *ui;
    OnlineQuery* query; // for online updates of crdt
};

#endif // MAINWINDOW_H