#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <../data_structure/remotecursor.h>
#include "../data_structure/action.h"
#include <data_structure/crdt.h>

class myTextEdit : public QTextEdit
{
public:
    myTextEdit(QWidget *,int user_id);

    void paintEvent(QPaintEvent *e);
    void createCursor(int pos, QString text, QColor color);
    void addCursor(RemoteCursor *cursor);
    void addAction(int cursorPos, int numChars, QString str, ActionType = Insertion);
    void addAction(int cursorPos, int numChars, ActionType = Deletion);
    void addAction(int cursorPos, int numChars, bool formatBoolean, TextFormatType, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, int index, TextFormatType, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, BlockFormatType, ActionType = BlockFormatting);
    void doReceivedActions();


    std::list<Action> toDoList;
    QStringList getFontSizes() const;
    QStringList getFontFamilies() const;

private:
    std::list<RemoteCursor*> cursorsList;
  //  std::list<Action> toSendList;
    QStringList fontSizes;
    QStringList fontFamilies;
    QTextCursor* hiddenCursor;
    int user_id;
};

#endif // MYTEXTEDIT_H
