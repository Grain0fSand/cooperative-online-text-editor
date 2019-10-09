#ifndef USERTAG_H
#define USERTAG_H

#include <QAbstractItemDelegate>
#include <QPainter>

class UserTag : public QAbstractItemDelegate
{

public:
    explicit UserTag(QWidget *parent = nullptr);
    ~UserTag() = default;

    UserTag(const UserTag &source) :
        userUsername(source.userUsername),
        userColor(source.userColor),
        userAvatar(source.userAvatar),
        userStatus(source.userStatus)
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

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void setUsername(QString);
    void setAvatar(QPixmap);
    void setUserStatus(bool);
    void setUserColor(QColor);
    QString getUsername();
    QPixmap getAvatar();
    bool getStatus();
    QColor getUserColor();

private:
    QString userUsername;
    QPixmap userAvatar;
    bool userStatus;
    QColor userColor;
};

#endif // USERTAG_H
