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
    LoginWindow(const LoginWindow&) = delete;
    LoginWindow& operator=(const LoginWindow&) = delete;

    static LoginWindow& getInstance(){
        static LoginWindow instance;

        return instance;
    }

    bool isLoginCorrect();
    static QString generateBlob(const QString& avatar_path);

public slots:
    void tryLogin();
    void tryRegister();
    void getRegisterResponse(bool,QString);
    void slowClose();
    void switchFrame(int direction=0);
    void changeYourUsername();
    void changeYourAvatar();
    void createDocument();
    void openDocument();
    void requestURI();
    void resetInputFields();

private:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    Ui::LoginWindow *ui;
    bool loginCorrect=false;
};

#endif // LOGINWINDOW_H
