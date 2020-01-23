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

    void paintEvent(QPaintEvent *e) override;
    void createRemoteCursor(int id, int pos, QString text, QColor color);
    void clearRemoteCursorList();
    void updateRemoteCursorPosition(int id, int pos);
    int doReceivedAction(const Action& action, int ownerId, const std::vector<int>& all_pos);
    const QString &getDocumentName() const;
    void setDocumentName(const QString &documentName);
    QStringList getFontSizes() const;
    QStringList getFontFamilies() const;
    static QColor chooseColorTextFromBackground(const QColor& background);
    void clearDocument();

public slots:
    void colorText(bool checked);

private:

    std::list<RemoteCursor> cursorsList;
    QStringList fontSizes;
    QStringList fontFamilies;
    QTextCursor* hiddenCursorForText;
    QTextCursor* hiddenCursorForColors;
    QString documentName;
};

#endif // MYTEXTEDIT_H
