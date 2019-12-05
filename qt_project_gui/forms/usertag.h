#ifndef USERTAG_H
#define USERTAG_H

#include <QAbstractItemDelegate>
#include <QPainter>
#include <data_structure/exchangeable.h>

class UserTag : public QAbstractItemDelegate
{

public:
    explicit UserTag(QWidget *parent = nullptr);
    ~UserTag() = default;

    UserTag(const UserTag &source) :
        userId(source.userId),
        userUsername(source.userUsername),
        userAvatar(source.userAvatar),
        userStatus(source.userStatus),
        userColor(source.userColor)
    {
    }

    UserTag& operator=(const UserTag &source) {
        if(this != &source) {
            this->userUsername = source.userUsername;
            this->userColor = source.userColor;
            this->userAvatar = source.userAvatar;
            this->userStatus = source.userStatus;
        }
        return *this;
    }

    // for std::find method
    bool operator==(const UserTag& other) const {
        return this->userUsername == other.userUsername;
    }

    // for std::find with exchangeble_data::user element comparing
    bool operator==(const exchangeable_data::user& other) const {
        return this->userId == std::stoi(other.id);
    }

    /* probably we need not to use that if all is correct
    // for std::sort with exchangable_data::user
    bool operator<(const exchangeable_data::user& other) const {
        return this->userUsername < QString::fromStdString(other.username);
    }
    */

    // for std::sort
    bool operator<(const UserTag& other) const {
        return this->userUsername < other.userUsername;
    }

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void setUserId(int);
    void setUsername(QString);
    void setAvatar(QPixmap);
    void setUserStatus(bool);
    void setUserColor(QColor);
    int getUserId();
    QString getUsername() const; // always better set explicitly const, so the compiler check if you access to modify some variables
    QPixmap getAvatar();
    bool getStatus();
    QColor getUserColor();

private:
    int userId;
    QString userUsername;
    QPixmap userAvatar;
    bool userStatus;
    QColor userColor;
};

#endif // USERTAG_H
