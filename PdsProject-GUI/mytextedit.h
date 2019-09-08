#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <remotecursor.h>
#include "action.h"

class myTextEdit : public QTextEdit
{
public:
    myTextEdit(QWidget *);

    void paintEvent(QPaintEvent *e);
    void createCursor(int pos, QString text, QColor color);
    void addCursor(RemoteCursor *cursor);
    void addAction(int cursorPos, int numChars, QString str, ActionType = Insertion);
    void addAction(int cursorPos, int numChars, ActionType = Deletion);
    void addAction(int cursorPos, int numChars, bool formatBoolean, TextFormatType, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, int index, TextFormatType, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, BlockFormatType, ActionType = BlockFormatting);
    void removeAction();
    void receiveAction(Action action);

private:
    std::list<RemoteCursor*> cursorsList;
    std::list<Action> toDoList;
};

#endif // MYTEXTEDIT_H
