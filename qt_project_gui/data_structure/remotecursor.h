#ifndef REMOTECURSOR_H
#define REMOTECURSOR_H

#include <QObject>
#include <QColor>

class RemoteCursor
{
public:
    RemoteCursor() :
        x(0),
        y(0),
        verticalOffset(0),
        text(""),
        color(Qt::black)
    {
        //nothing else to add (for now...)
    }

    void setX(int x) { this->x = x; }
    int getX() { return this->x; }
    void setY(int y) { this->y = y; }
    int getY() { return this->y; }
    void setVerticalOffset(int offset) { this->verticalOffset = offset; }
    int getVerticalOffset() { return this->verticalOffset; }
    void setText(QString text) { this->text = text; }
    QString getText() { return this->text; }
    void setColor(QColor color) { this->color = color; }
    QColor getColor() { return this->color; }

private:
    int x;
    int y;
    int verticalOffset;
    QString text;
    QColor color;
};

#endif // REMOTECURSOR_H
