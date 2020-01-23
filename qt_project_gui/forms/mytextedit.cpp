#include "mytextedit.h"
#include "mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>


MyTextEdit::MyTextEdit(QWidget *parent)
{
    this->setCurrentFont(QFont("Calibri",11,-1,false));
    this->document()->setDocumentMargin(30);
    this->hiddenCursorForText = new QTextCursor(this->document());
    this->hiddenCursorForColors = new QTextCursor(this->document());
    this->fontSizes << "8" << "9" << "10" << "11" << "12" <<
                       "14" << "16" << "18" << "20" << "22" <<
                       "24" << "26" << "28" << "36" << "48" << "72";
    this->fontFamilies << "Arial" << "Arial Black" << "Berlin Sans FB" << "Calibri" << "Century Gothic" <<
                          "Consolas" << "Constantia" << "Forte" << "Freestyle Script" << "Georgia" <<
                          "Gill Sans MT" << "Harrington" << "Informal Roman" << "Lucida Calligraphy" <<
                          "Palatino Linotype" << "Segoe Script" << "Tahoma" << "Times New Roman" <<
                          "Verdana" << "Vivaldi";
}

MyTextEdit::~MyTextEdit() {}

void MyTextEdit::paintEvent(QPaintEvent *e) {
    if(!cursorsList.empty()) {
        QPainter painter(this->viewport());
        QFont font("Gill Sans MT",9,QFont::Bold);

        QTextCursor tmpCursor(this->document());
        QRect cursorRect;

        std::list<RemoteCursor>::iterator it;
        for(it=this->cursorsList.begin(); it!=this->cursorsList.end(); it++) {
            tmpCursor.setPosition(it->getPos());
            cursorRect = this->cursorRect(tmpCursor);

            painter.setPen(Qt::black);
            QString userName(it->getText());

            QRect rect(cursorRect.x(),cursorRect.y()-12,10+6*userName.length(),14);
            painter.drawRect(rect);
            painter.fillRect(rect,it->getColor());

            QLine line(cursorRect.x(),cursorRect.y(),cursorRect.x(),cursorRect.y()+cursorRect.height()-2);
            painter.setPen(it->getColor());
            painter.drawLine(line);

            painter.setPen(MyTextEdit::chooseColorTextFromBackground(it->getColor()));
            painter.setFont(font);
            painter.drawText(cursorRect.x()+4,cursorRect.y()-1,userName);
        }
    }
    this->document()->blockSignals(true);
    QTextEdit::paintEvent(e);
    this->viewport()->update();
    this->document()->blockSignals(false);
}

int MyTextEdit::doReceivedAction(const Action& action, int ownerId, const std::vector<int>& all_pos )
{
    int ptr_start = 0, ptr_end, del_chars = 0, n = all_pos.size();    //ptr start and ptr end are for calculating ranges of subsequent positions in all_pos
    this->document()->blockSignals(true);
    while (ptr_start < n) {
        //find range of subsequent positions and apply the action (for efficiency)
        ptr_end = ptr_start;
        this->hiddenCursorForText->setPosition(all_pos[ptr_start] + (all_pos[ptr_start] == -1) - del_chars);
        while (ptr_end < n - 1 && all_pos[ptr_end] == all_pos[ptr_end + 1] - 1)
            ++ptr_end;
        switch(action.getActionType())
        {
            case NoActionType:
                qDebug() << "Invalid action to do";
                break;
            case Insertion: {
                this->hiddenCursorForText->insertText(action.getChars());
                this->hiddenCursorForText->setPosition(all_pos[ptr_start]);
                this->hiddenCursorForText->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, action.getChars().size());
                QTextCharFormat format;


                action.getBold() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                format.setFontItalic(action.getItalic());
                format.setFontUnderline(action.getUnderlined());
                format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                format.setFontPointSize(this->fontSizes.at(action.getFontSize()).toInt());

                if(colorFeatureActive) {
                    QColor color;
                    color = SessionData::accessToSessionData().userColorMap[ownerId];
                    format.setBackground(QBrush(color));
                    format.setForeground(QBrush(chooseColorTextFromBackground(color)));
                }

                this->hiddenCursorForText->setCharFormat(format);

                QTextBlockFormat blockFormat;
                switch(action.getBlockFormat())
                {
                    case NoBlockFormatType:
                        qDebug()  << "Invalid block formatting to do";
                        break;
                    case AlignLeft:
                        blockFormat.setAlignment(Qt::AlignLeft);
                        break;
                    case AlignCenter:
                        blockFormat.setAlignment(Qt::AlignCenter);
                        break;
                    case AlignRight:
                        blockFormat.setAlignment(Qt::AlignRight);
                        break;
                    case AlignJustify:
                        blockFormat.setAlignment(Qt::AlignJustify);
                        break;
                }
                this->hiddenCursorForText->setBlockFormat(blockFormat);
                break;
            }
            case BlockFormatting:
            {
                this->hiddenCursorForText->setPosition(all_pos[ptr_start] + 1);
                this->hiddenCursorForText->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end - ptr_start);
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
                this->hiddenCursorForText->setBlockFormat(format);
                break;
            }
            case TextFormatting:
            {
                this->hiddenCursorForText->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, ptr_end + 1 - ptr_start);
                QTextCharFormat format;

                switch(action.getSelection()) {
                    case 0:
                        action.getBold() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                        break;
                    case 1:
                        format.setFontItalic(action.getItalic());
                        break;
                    case 2:
                        format.setFontUnderline(action.getUnderlined());
                        break;
                    case 3:
                        format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                        break;
                    case 4:
                        format.setFontPointSize(this->fontSizes.at(action.getFontSize()).toInt());
                        break;
                }
                this->hiddenCursorForText->mergeCharFormat(format);
                break;
            }
            case Deletion:
                this->hiddenCursorForText->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end + 1 - ptr_start);
                del_chars += ptr_end + 1 - ptr_start;
                this->hiddenCursorForText->removeSelectedText();
                break;
            default:
                break;
        }
        ptr_start = ptr_end + 1;
    }
    SessionData::accessToSessionData().mainWindowPointer->checkTextProperty();
    this->document()->blockSignals(false);
    this->hiddenCursorForText->movePosition(QTextCursor::End);
    return this->hiddenCursorForText->position();
}

QStringList MyTextEdit::getFontSizes() const
{
    return fontSizes;
}

QStringList MyTextEdit::getFontFamilies() const
{
    return fontFamilies;
}

void MyTextEdit::colorText(bool checked)
{
    this->colorFeatureActive = checked;
    this->document()->blockSignals(true);

    if(checked) {
        std::vector<SymbolId> *list = &Crdt::getInstance().getSymbolList();
        int myId = std::stoi(SessionData::accessToSessionData().userId);
        int size = SessionData::accessToSessionData().myTextEditPointer->document()->characterCount()-1;
        QColor color;
        int start=0, end=0;

        std::vector<SymbolId>::iterator it;
        for(it=list->begin()+1; it<list->end() || end==size; it++) {

            int currentId = it->getSymbolId().second;
            start = end;

            do {
                if(!it->is_hidden()) {
                    end++;
                    if(end == size) break;
                }
                it++;
            } while(it->getSymbolId().second == currentId);

            if(currentId!=myId && start != end)
            {
                this->hiddenCursorForColors->setPosition(start);
                this->hiddenCursorForColors->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end-start);
                color = SessionData::accessToSessionData().userColorMap[currentId];
                QTextCharFormat format;
                format.setBackground(QBrush(color));
                format.setForeground(chooseColorTextFromBackground(color));
                this->hiddenCursorForColors->mergeCharFormat(format);
            }
            it--;
        }
    }
    else {
        QTextCharFormat format;
        format.setBackground(QBrush(Qt::black,Qt::NoBrush));
        format.setForeground(QBrush(Qt::black));
        this->hiddenCursorForColors->select(QTextCursor::Document);
        this->hiddenCursorForColors->mergeCharFormat(format);
    }

    this->document()->blockSignals(false);
}

QColor MyTextEdit::chooseColorTextFromBackground(const QColor& background) {
    if((background.red()*299 + background.green()*587 + background.blue()*114)/1000 < 123)
        return {Qt::white};
    return {Qt::black};
}

void MyTextEdit::clearDocument() {
    SessionData::accessToSessionData().skipChanges = true;
    this->document()->clear();
    SessionData::accessToSessionData().skipChanges = false;
}

void MyTextEdit::createRemoteCursor(int id, int pos, QString text, QColor color) {

    RemoteCursor newCursor;
    newCursor.setId(id);
    newCursor.setPos(pos);
    newCursor.setText(text);
    newCursor.setColor(color);

    this->cursorsList.push_back(newCursor);
}

void MyTextEdit::updateRemoteCursorPosition(int id, int pos) {
    for(auto& cursor : this->cursorsList) {
        if(cursor.getId() == id)
            cursor.setPos(pos);
    }
}

void MyTextEdit::clearRemoteCursorList() {
    this->cursorsList.clear();
}

const QString &MyTextEdit::getDocumentName() const {
    return documentName;
}

void MyTextEdit::setDocumentName(const QString &documentName) {
    MyTextEdit::documentName = documentName;
}
