#ifndef USERTAG_H
#define USERTAG_H

#include <QAbstractItemDelegate>
#include <QPainter>

class UserTag : public QAbstractItemDelegate
{

public:
    explicit UserTag(QWidget *parent = nullptr);
    ~UserTag() = default;

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void setUsername(QString);
    void setAvatar(QPixmap);
    void setUserStatus(bool);
    void setUserColor(QString);
    QString getUsername();
    QPixmap getAvatar();
    bool getStatus();
    QString getUserColor();

    void chooseAvatar(); //TODO: to implement

signals:

public slots:

private:
    QString userUsername;
    QPixmap userAvatar;
    bool userStatus;
    QString userColor;
};

#endif // USERTAG_H
