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
    SessionData sessionData;
    LoginWindow(const LoginWindow&) = delete;
    LoginWindow& operator=(const LoginWindow&) = delete;

    static LoginWindow& getInstance(){
        static LoginWindow instance;

        return instance;
    }

    enum Frame {
        Registration,
        Login,
        PersonalPage
    };

    static bool validateEmail(QString email);
    bool isLoginCorrect();
    void showLoading(Frame frame);
    static QString generateEncodedImage(const QString& avatar_path);
    static QPixmap recoverImageFromEncodedString(const QString& code);
    Frame currentVisibleFrame();

public slots:
    void tryLogin();
    void tryRegister();
    void showRegisterResponse(bool goodResponse, QString responseText);
    void showLoginResponse(bool goodResponse, QString responseText, QString replyString);
    void showNewDocResponse(bool goodResponse, QString responseText, QString replyString);
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
    QStringList docsList;
};

#endif // LOGINWINDOW_H
