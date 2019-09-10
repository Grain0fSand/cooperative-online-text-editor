#include "remotecursor.h"
#include "../forms/mytextedit.h"

RemoteCursor::RemoteCursor() :
    x(0),
    y(0),
    verticalOffset(0),
    text(""),
    color(Qt::black)
{
    //nothing else to add (for now...)
}

void RemoteCursor::setX(int x) { this->x = x; }
int RemoteCursor::getX() { return this->x; }

void RemoteCursor::setY(int y) { this->y = y; }
int RemoteCursor::getY() { return this->y; }

void RemoteCursor::setVerticalOffset(int offset) { this->verticalOffset = offset; }
int RemoteCursor::getVerticalOffset() { return this->verticalOffset; }

void RemoteCursor::setText(QString text) { this->text = text; }
QString RemoteCursor::getText() { return this->text; }

void RemoteCursor::setColor(QColor color) { this->color = color; }
QColor RemoteCursor::getColor() { return this->color; }
