#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <../data_structure/remotecursor.h>
#include "../data_structure/action.h"
#include <data_structure/crdt.h>
#include <QPointer>
#include <stack>
class MyTextEdit : public QTextEdit
{
public:
    MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();
    int previousSelection, previousPreviousSelection;
    bool colorFeatureActive = false;
    //MyTextEdit(const MyTextEdit &) = delete;
    //MyTextEdit& operator=(const MyTextEdit &) = delete;

    void paintEvent(QPaintEvent *e);
    void createCursor(int pos, QString text, QColor color);
    void addCursor(RemoteCursor *cursor);
    void addAction(int cursorPos, int numChars, QString str, ActionType = Insertion);
    void addAction(int cursorPos, int numChars, ActionType = Deletion);
    void addAction(int cursorPos, int numChars, bool bold, bool italic, bool underlined, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, BlockFormatType, ActionType = BlockFormatting);
    int doReceivedAction(const Action& action, int ownerId, const std::vector<int>& all_pos);
    const QString &getDocumentName() const;
    void setDocumentName(const QString &documentName);
    //std::list<Action> toDoList;
    QStringList getFontSizes() const;
    QStringList getFontFamilies() const;
    static QColor chooseColorTextFromBackground(QColor& background);
public slots:
    void colorText(bool checked);

private:

      std::list<RemoteCursor*> cursorsList;
  //  std::list<Action> toSendList;
   // static inline MyTextEdit* instance = nullptr;
    QStringList fontSizes;
    QStringList fontFamilies;
    QTextCursor* hiddenCursorForText;
    QTextCursor* hiddenCursorForColors;
    QString documentName;

    int user_id;
};

#endif // MYTEXTEDIT_H
