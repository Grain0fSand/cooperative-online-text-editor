#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QMovie>
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

    bool validateEmail(QString email);
    bool isLoginCorrect();
    void showLoading();
    static QString generateBlob(const QString& avatar_path);

public slots:
    void tryLogin();
    void tryRegister();
    void showRegisterResponse(bool good_response, QString response_text);
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
    bool loginCorrect;
};

#endif // LOGINWINDOW_H
