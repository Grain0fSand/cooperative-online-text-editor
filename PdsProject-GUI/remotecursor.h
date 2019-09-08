#ifndef REMOTECURSOR_H
#define REMOTECURSOR_H

#include <QObject>
#include <QColor>

class RemoteCursor
{
public:
    RemoteCursor();

    void setX(int x);
    int getX();
    void setY(int y);
    int getY();
    void setVerticalOffset(int offset);
    int getVerticalOffset();
    void setText(QString text);
    QString getText();
    void setColor(QColor color);
    QColor getColor();

private:
    int x;
    int y;
    int verticalOffset;
    QString text;
    QColor color;
};

#endif // REMOTECURSOR_H
