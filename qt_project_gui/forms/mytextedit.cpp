#include "mytextedit.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>


myTextEdit::myTextEdit(QWidget *,int user_id)
{
    this->setCurrentFont(QFont("Calibri",11,-1,false));
    this->document()->setDocumentMargin(11);
    this->hiddenCursor = new QTextCursor(this->document());
    this->fontSizes << "8" << "9" << "10" << "11" << "12" <<
                       "14" << "16" << "18" << "20" << "22" <<
                       "24" << "26" << "28" << "36" << "48" << "72";
    this->fontFamilies << "Arial" << "Arial Black" << "Berlin Sans FB" << "Calibri" << "Century Gothic" <<
                          "Consolas" << "Constantia" << "Freestyle Script" << "Georgia" << "Gill Sans MT" <<
                          "Informal Roman" << "Lucida Calligraphy" << "MS Shell Dlg 2" <<
                          "Palatino Linotype" << "Tahoma" << "Times New Roman" << "Verdana" << "Vivaldi";
    this->user_id = user_id;
    Crdt::getInstance().init(user_id);
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

void myTextEdit::addAction(int cursorPos, int numChars, QString chars, ActionType actionType) //insertion
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setChars(chars);

    Crdt::getInstance().sendActionToServer(action);
   // this->toSendList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, ActionType actionType) //deletion
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);

    Crdt::getInstance().sendActionToServer(action);
   // this->toSendList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, bool formatBoolean, TextFormatType formatType, ActionType actionType) //text formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setTextFormatBoolean(formatBoolean);
    action.setTextFormatType(formatType);

    Crdt::getInstance().sendActionToServer(action);
   // this->toSendList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, int index, TextFormatType formatType, ActionType actionType) //font formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setComboFontIndex(index);
    action.setTextFormatType(formatType);



    //nb: removed because the modification is sent immediatly after elaboration of crdt
    //and the responsability of send online is of the class crdt
    //this->toSendList.push_front(action);
}

void myTextEdit::addAction(int cursorPos, int numChars, BlockFormatType formatType, ActionType actionType) //block formatting
{
    Action action;
    action.setActionType(actionType);
    action.setCursorPos(cursorPos);
    action.setNumChars(numChars);
    action.setBlockFormatType(formatType);

  //  this->toSendList.push_front(action);
}

void myTextEdit::doReceivedActions()
{
    this->document()->blockSignals(true);
    while(!toDoList.empty()) {
        auto action = toDoList.back();
        this->hiddenCursor->setPosition(action.getCursorPos());
        switch(action.getActionType())
        {
            case NoActionType:
                qDebug() << "Invalid action to do";
                break;
            case Insertion:
                this->hiddenCursor->insertText(action.getChars());
                break;
            case Deletion:
                this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,action.getNumChars());
                this->hiddenCursor->removeSelectedText();
                break;
            case TextFormatting:
            {
                this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,action.getNumChars());
                QTextCharFormat format;
                switch(action.getTextFormatType())
                {
                    case NoTextFormatType:
                        qDebug()  << "Invalid text formatting to do";
                        break;
                    case Bold:
                        action.getTextFormatBoolean() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                        break;
                    case Italic:
                        format.setFontItalic(action.getTextFormatBoolean());
                        break;
                    case Underlined:
                        format.setFontUnderline(action.getTextFormatBoolean());
                        break;
                    case FontFamily:
                        format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                        break;
                    case FontSize:
                        format.setFontPointSize(this->fontSizes.at(action.getComboFontIndex()).toInt());
                        break;
                }
                this->hiddenCursor->mergeCharFormat(format);
                break;
            }
            case BlockFormatting:
            {
                this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,action.getNumChars());
                QTextBlockFormat format;
                switch(action.getBlockFormatType())
                {
                    case NoBlockFormatType:
                        qDebug()  << "Invalid block formatting to do";
                        break;
                    case AlignLeft:
                        format.setAlignment(Qt::AlignLeft);
                        break;
                    case AlignCenter:
                        format.setAlignment(Qt::AlignCenter);
                        break;
                    case AlignRight:
                        format.setAlignment(Qt::AlignRight);
                        break;
                    case AlignJustify:
                        format.setAlignment(Qt::AlignJustify);
                        break;
                }
                this->hiddenCursor->mergeBlockFormat(format);
                break;
            }
        }
        toDoList.pop_back();
    }
    this->document()->blockSignals(false);
}

QStringList myTextEdit::getFontSizes() const
{
    return fontSizes;
}

QStringList myTextEdit::getFontFamilies() const
{
    return fontFamilies;
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