#include "mytextedit.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>

myTextEdit::myTextEdit(QWidget *)
{
    this->setCurrentFont(QFont("Calibri",11,-1,false));
    this->document()->setDocumentMargin(11);
}

void myTextEdit::paintEvent(QPaintEvent *e) {

    if(!cursorsList.empty()) {
        QPainter painter(this->viewport());
        QFont font("Gill Sans MT",9,QFont::Bold);

        std::list<RemoteCursor*>::iterator it;
        for(it=this->cursorsList.begin(); it!=this->cursorsList.end(); it++) {

            painter.setPen(Qt::black);

            QString userName((*it)->getText());

            QRect rect((*it)->getX(),(*it)->getY()-12,10+6*userName.length(),14);
            painter.drawRect(rect);
            painter.fillRect(rect,(*it)->getColor());

            QLine line((*it)->getX(),(*it)->getY(),(*it)->getX(),(*it)->getY()+(*it)->getVerticalOffset()-2);
            painter.setPen((*it)->getColor());
            painter.drawLine(line);

            painter.setPen(Qt::white);
            painter.setFont(font);
            painter.drawText((*it)->getX()+4,(*it)->getY()-1,userName);
        }

    }

    QTextEdit::paintEvent(e);
}

void myTextEdit::addCursor(RemoteCursor *cursor)
{
    this->cursorsList.push_back(cursor);
}

void myTextEdit::addAction(int pos, int numChars, QString str, ActionType actionType) //insertion
{

    Action action;
    action.setActionType(actionType);
    action.setCursorPos(pos);
    action.setNumChars(numChars);
    action.setChars(str);
    this->toDoList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, ActionType actionType) //deletion
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);

    this->toDoList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, bool formatBoolean, TextFormatType formatType, ActionType actionType) //text formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setTextFormatBoolean(formatBoolean);
    action.setTextFormatType(formatType);

    this->toDoList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, int index, TextFormatType formatType, ActionType actionType) //text formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setComboFontIndex(index);
    action.setTextFormatType(formatType);

    this->toDoList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, BlockFormatType formatType, ActionType actionType) //block formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setBlockFormatType(formatType);

    this->toDoList.push_front(action);
}

void myTextEdit::removeAction()
{
    this->toDoList.pop_front();
}

void myTextEdit::createCursor(int pos, QString text, QColor color) {

    QTextCursor tmpCursor(this->document());
    tmpCursor.setPosition(pos);
    QRect cursorRect = this->cursorRect(tmpCursor);

    RemoteCursor* newCursor = new RemoteCursor;
    newCursor->setX(cursorRect.x());
    newCursor->setY(cursorRect.y());
    newCursor->setVerticalOffset(cursorRect.height());
    newCursor->setText(text);
    newCursor->setColor(color);

    this->addCursor(newCursor);
}
