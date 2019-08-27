#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    bool isLoginCorrect();

public slots:
    void tryLogin();
    void slowClose();
    void switchFrame();

private:
    Ui::LoginWindow *ui;
    bool loginCorrect=false;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int mouseClickXCoordinate;
    int mouseClickYCoordinate;
};

#endif // LOGINWINDOW_H
