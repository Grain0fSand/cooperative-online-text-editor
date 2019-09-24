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
    myTextEdit(const myTextEdit&) = delete;
    myTextEdit& operator=(const myTextEdit&) = delete;

    static myTextEdit& getInstance(){
        static myTextEdit instance;

        return instance;
    }

    void paintEvent(QPaintEvent *e);
    void createCursor(int pos, QString text, QColor color);
    void addCursor(RemoteCursor *cursor);
    void addAction(int cursorPos, int numChars, QString str, ActionType = Insertion);
    void addAction(int cursorPos, int numChars, ActionType = Deletion);
    void addAction(int cursorPos, int numChars, bool bold, bool italic, bool underlined, ActionType = TextFormatting);
    void addAction(int cursorPos, int numChars, BlockFormatType, ActionType = BlockFormatting);
    void doReceivedAction(Action& action, std::vector<int>& all_pos);

    std::list<Action> toDoList;
    QStringList getFontSizes() const;
    QStringList getFontFamilies() const;

public slots:
    void colorText(bool checked);

private:
    myTextEdit(QWidget *parent = nullptr);
    ~myTextEdit();

    std::list<RemoteCursor*> cursorsList;
  //  std::list<Action> toSendList;
    std::vector<QString> textColorsList;
    QStringList fontSizes;
    QStringList fontFamilies;
    QTextCursor* hiddenCursor;
    QString documentName;

public:
    const QString &getDocumentName() const;
    void setDocumentName(const QString &documentName);

private:
    int user_id;
};

#endif // MYTEXTEDIT_H
