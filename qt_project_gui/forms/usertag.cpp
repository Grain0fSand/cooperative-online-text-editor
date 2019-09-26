#include "usertag.h"
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

UserTag::UserTag(QWidget *parent)
{
}

void UserTag::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{
    painter->setRenderHint(QPainter::Antialiasing);

    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //background
    //alternating colors
    painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
    painter->drawRect(r);

    //border
    painter->setPen(linePen);
    painter->drawLine(r.topLeft(),r.topRight());
    painter->drawLine(r.topRight(),r.bottomRight());
    painter->drawLine(r.bottomLeft(),r.bottomRight());
    painter->drawLine(r.topLeft(),r.bottomLeft());

    painter->setPen(fontPen);

    //username, status, avatar, led and color
    QString title = index.data(Qt::UserRole + 1).toString();
    QString description = index.data(Qt::UserRole + 2).toString();
    QIcon avatar = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole + 3)));
    QIcon led = QIcon(qvariant_cast<QPixmap>(index.data(Qt::UserRole + 4)));
    QColor color = qvariant_cast<QColor>(index.data(Qt::UserRole + 5));

    //username
    r = option.rect.adjusted(70, 0, -10, -30);
    painter->setFont( QFont( "Gill Sans MT", 14, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);

    //status
    r = option.rect.adjusted(88, 30, -10, 0);
    painter->setFont( QFont( "Tahoma", 11, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);

    //logo
    r = option.rect.adjusted(10, 10, -10, -10);
    avatar.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);

    //led
    r = option.rect.adjusted(70, 34, -15, -15);
    led.paint(painter, r, Qt::AlignVCenter|Qt::AlignLeft);

    //color
    QPainterPath path;
    path.addRoundedRect(option.rect.adjusted(200, 34, -10, -15), 5, 5);
    painter->drawPath(path);
    painter->fillPath(path, QBrush(color));

    QPen pen(Qt::black, 1);
    painter->setPen(pen);
}

QSize UserTag::sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index ) const{
    return QSize(200, 60); // very dumb value
}

void UserTag::setUsername(QString newName)
{
    this->userUsername = newName;
}

void UserTag::chooseAvatar()
{
    //to implement?
}

void UserTag::setUserStatus(bool newStatus)
{
    this->userStatus = newStatus;
}

QString UserTag::getUsername()
{
    return this->userUsername;
}

QPixmap UserTag::getAvatar()
{
    return this->userAvatar;
}

bool UserTag::getStatus()
{
    return this->userStatus;
}




