#include "mytextedit.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>


myTextEdit::myTextEdit(QWidget *parent)
{
    this->setParent(parent);

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
    this->user_id = 0;
    //testing the coloring feature
    this->textColorsList.emplace_back("#6DFF4C");
    this->textColorsList.emplace_back("#6DFF4C");
    this->textColorsList.emplace_back("#FF0000");
    this->textColorsList.emplace_back("#6DFF4C");
    this->textColorsList.emplace_back("#6DFF4C");

    Crdt::getInstance().init(user_id);
}

myTextEdit::~myTextEdit() {
    delete hiddenCursor;
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

//void myTextEdit::addAction(int cursorPos, int numChars, QString chars, ActionType actionType) //insertion
//{
//    Action action;
//    action.setActionType(actionType);
//    action.setCursorPos(cursorPos);
//    action.setNumChars(numChars);
//    action.setChars(chars);
//
//    Crdt::getInstance().sendActionToServer(action);
//   // this->toSendList.push_front(action);
//}
//
//void myTextEdit::addAction(int cursorPos, int numChars, ActionType actionType) //deletion
//{
//    Action action;
//    action.setActionType(actionType);
//    action.setCursorPos(cursorPos);
//    action.setNumChars(numChars);
//
//    Crdt::getInstance().sendActionToServer(action);
//   // this->toSendList.push_front(action);
//}
//
//void myTextEdit::addAction(int cursorPos, int numChars, bool formatBoolean, TextFormatType formatType, ActionType actionType) //text formatting
//{
//    Action action;
//    action.setActionType(actionType);
//    action.setCursorPos(cursorPos);
//    action.setNumChars(numChars);
//    action.setTextFormatBoolean(formatBoolean);
//    action.setTextFormatType(formatType);
//
//    Crdt::getInstance().sendActionToServer(action);
//   // this->toSendList.push_front(action);
//}
//
//void myTextEdit::addAction(int cursorPos, int numChars, int index, TextFormatType formatType, ActionType actionType) //font formatting
//{
//    Action action;
//    action.setActionType(actionType);
//    action.setCursorPos(cursorPos);
//    action.setNumChars(numChars);
//    action.setComboFontIndex(index);
//    action.setTextFormatType(formatType);
//
//
//
//    //nb: removed because the modification is sent immediatly after elaboration of crdt
//    //and the responsability of send online is of the class crdt
//    //this->toSendList.push_front(action);
//}
//
//void myTextEdit::addAction(int cursorPos, int numChars, BlockFormatType formatType, ActionType actionType) //block formatting
//{
//    Action action;
//    action.setActionType(actionType);
//    action.setCursorPos(cursorPos);
//    action.setNumChars(numChars);
//    action.setBlockFormatType(formatType);
//
//  //  this->toSendList.push_front(action);
//}
//
void myTextEdit::doReceivedAction(Action& action, std::vector<int>& all_pos )
{
   // this->document()->blockSignals(true);
        int ptr_start, ptr_end = 0, n = all_pos.size();    //ptr start and ptr end are for calculating ranges of subsequent positions in all_pos

        while (ptr_start < n) {
            //find range of subsequent positions and apply the action (for efficiency)
            ptr_start = ptr_end;
            this->hiddenCursor->setPosition(all_pos[ptr_start]);
            while (ptr_end < n && all_pos[ptr_end] == all_pos[ptr_end + 1] - 1)
                ++ptr_end;

            switch(action.getActionType())
            {
                case NoActionType:
                    qDebug() << "Invalid action to do";
                    break;
                case Insertion:
                    this->hiddenCursor->insertText(action.getChars().mid(ptr_start, ptr_end + 1 - ptr_start));
                    break;
                case Deletion:
                    this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end + 1 - ptr_start);
                    this->hiddenCursor->removeSelectedText();
                    break;
                case TextFormatting:
                {
                    this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, ptr_end + 1 - ptr_start);
                    QTextCharFormat format;

                    action.getBold() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                    format.setFontItalic(action.getItalic());
                    format.setFontUnderline(action.getUnderlined());
                    format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                    format.setFontPointSize(this->fontSizes.at(action.getComboFontIndex()).toInt());
                    this->hiddenCursor->mergeCharFormat(format);
                    break;
                }
                case BlockFormatting:
                {
                    this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end + 1 - ptr_start);
                    QTextBlockFormat format;
                    switch(action.getBlockFormat())
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
            ++ptr_end;
        }
}

QStringList myTextEdit::getFontSizes() const
{
    return fontSizes;
}

QStringList myTextEdit::getFontFamilies() const
{
    return fontFamilies;
}

void myTextEdit::colorText(bool checked)
{
    QTextCharFormat format;
    if(checked) {
        int size = this->textColorsList.size();
        for(int i=0; i<size; i++) {
            int n = i;
            this->hiddenCursor->setPosition(i);
            QString currentColor = this->textColorsList.at(i);
            while(this->textColorsList.at(i+1) == currentColor) {
                i++;
                if(i==(size-1)) break;
            }
            this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, i-n+1);
            format.setBackground(QBrush(QColor(this->textColorsList.at(n))));
            this->hiddenCursor->mergeCharFormat(format);
        }
    }
    else {
        format.setBackground(QBrush(Qt::black,Qt::NoBrush));
        this->hiddenCursor->select(QTextCursor::Document);
        this->hiddenCursor->mergeCharFormat(format);
    }

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

const QString &myTextEdit::getDocumentName() const {
    return documentName;
}

void myTextEdit::setDocumentName(const QString &documentName) {
    myTextEdit::documentName = documentName;
}
