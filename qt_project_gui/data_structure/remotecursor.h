#ifndef REMOTECURSOR_H
#define REMOTECURSOR_H

#include <QObject>
#include <QColor>

class RemoteCursor
{
public:
    RemoteCursor() :
        pos(0),
        text(""),
        color(Qt::black)
    {
        //nothing else to add (for now...)
    }
    void setId(int id) { this->id = id; }
    int getId() { return this->id; }
    void setPos(int pos) { this->pos = pos; }
    int getPos() { return this->pos; }
    void setText(QString text) { this->text = text; }
    QString getText() { return this->text; }
    void setColor(QColor color) { this->color = color; }
    QColor getColor() { return this->color; }

private:
    int id;
    int pos;
    QString text;
    QColor color;
};

#endif // REMOTECURSOR_H
