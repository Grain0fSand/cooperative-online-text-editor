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
    void chooseAvatar();
    void setUserStatus(bool);
    QString getUsername();
    QPixmap getAvatar();
    bool getStatus();

signals:

public slots:

private:
    QString userUsername;
    QPixmap userAvatar;
    bool userStatus;
};

#endif // USERTAG_H
