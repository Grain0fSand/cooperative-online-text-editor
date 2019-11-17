#include "mytextedit.h"
#include "mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QPaintEvent>


MyTextEdit::MyTextEdit(QWidget *parent)
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
}

//TODO: check delete hiddenCursor
MyTextEdit::~MyTextEdit() {
   // delete hiddenCursor;
}

void MyTextEdit::paintEvent(QPaintEvent *e) {

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

void MyTextEdit::addCursor(RemoteCursor *cursor)
{
    this->cursorsList.push_back(cursor);
}

void MyTextEdit::doReceivedAction(const Action& action, const std::vector<int>& all_pos )
{
    int ptr_start = 0, ptr_end, n = all_pos.size();    //ptr start and ptr end are for calculating ranges of subsequent positions in all_pos
    while (ptr_start < n) {
        //find range of subsequent positions and apply the action (for efficiency)
        ptr_end = ptr_start;
        this->hiddenCursor->setPosition(all_pos[ptr_start]);
        while (ptr_end < n - 1 && all_pos[ptr_end] == all_pos[ptr_end + 1] - 1)
            ++ptr_end;
        this->document()->blockSignals(true);
        switch(action.getActionType())
        {
            case NoActionType:
                qDebug() << "Invalid action to do";
                break;
            case Insertion: {
                this->hiddenCursor->insertText(action.getChars());
                this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, action.getChars().size());
                QTextCharFormat format;

                action.getBold() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                format.setFontItalic(action.getItalic());
                format.setFontUnderline(action.getUnderlined());
                format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                format.setFontPointSize(this->fontSizes.at(action.getFontSize()).toInt());
                this->hiddenCursor->setCharFormat(format);

                //this is because only first paragraph gets block alignment when copying multiple paragraphs
                std::string st = action.getChars().toStdString();
                unsigned int i;
                for (i = 0; i < st.size(); ++i) {
                    if (st[i] == '\n')
                        break;
                }
                this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, all_pos[ptr_start] + i + 1);
                QTextBlockFormat bFormat;
                switch(action.getBlockFormat())
                {
                    case NoBlockFormatType:
                        qDebug()  << "Invalid block formatting to do";
                        break;
                    case AlignLeft:
                        bFormat.setAlignment(Qt::AlignLeft);
                        break;
                    case AlignCenter:
                        bFormat.setAlignment(Qt::AlignCenter);
                        break;
                    case AlignRight:
                        bFormat.setAlignment(Qt::AlignRight);
                        break;
                    case AlignJustify:
                        bFormat.setAlignment(Qt::AlignJustify);
                        break;
                }
                this->hiddenCursor->setBlockFormat(bFormat);
                if (i != st.size()) {
                    this->hiddenCursor->setPosition(all_pos[ptr_start] + i + 1);
                    this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, st.size() - i);
                    QTextBlockFormat bFormat;
                    bFormat.setAlignment(Qt::AlignLeft);
                    this->hiddenCursor->setBlockFormat(bFormat);
                }
                break;
            }
            case BlockFormatting:
            {
                this->hiddenCursor->setPosition(all_pos[ptr_start] + 1);
                this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end - ptr_start);
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
                this->hiddenCursor->setBlockFormat(format);
                break;
            }
            case TextFormatting:
            {
                this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, ptr_end + 1 - ptr_start);
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
                this->hiddenCursor->mergeCharFormat(format);
                break;
            }
            case Deletion:
                this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end + 1 - ptr_start);
                this->hiddenCursor->removeSelectedText();
                break;
            default:
                break;
        }
        this->document()->blockSignals(false);
        ptr_start = ptr_end + 1;
    }
    MainWindow::getInstance().checkTextProperty();
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
    if(checked) {
        std::vector<SymbolId> *list = &Crdt::getInstance().getSymbolList();
        int myId = std::stoi(MainWindow::getInstance().sessionData.userId);
        int size = MyTextEdit::getInstance().document()->characterCount()-1;
        QColor color;
        int start=0, end=0;

        std::vector<SymbolId>::iterator it;
        for(it=list->begin()+1; it<list->end(); it++) {
            int uid = it->getSymbolId().second;
            if(uid==myId) {
                start++;
                end++;
                continue;
            }
            else if(it->is_hidden()) {
                continue;
            }
            else {
                end = start;
                this->hiddenCursor->setPosition(start);
                color = (MainWindow::getInstance().sessionData.userColorMap[uid]);
                while(it->getSymbolId().second == uid) {
                    if(!it->is_hidden()) {
                        end++;
                        if(end==(size-1)) break;
                    }
                    it++;
                }
                this->hiddenCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, end-start);

                QTextCharFormat format;
                format.setBackground(QBrush(color));
                if((color.red()*299 + color.green()*587 + color.blue()*114)/1000 < 123)
                    format.setForeground(QBrush(QColor(Qt::white)));
                this->hiddenCursor->mergeCharFormat(format);

                start=end;
                it--;
            }
        }
    }
    else {
        QTextCharFormat format;
        format.setBackground(QBrush(Qt::black,Qt::NoBrush));
        format.setForeground(QBrush(Qt::black));
        this->hiddenCursor->select(QTextCursor::Document);
        this->hiddenCursor->mergeCharFormat(format);
    }
}

void MyTextEdit::createCursor(int pos, QString text, QColor color) {

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

const QString &MyTextEdit::getDocumentName() const {
    return documentName;
}

void MyTextEdit::setDocumentName(const QString &documentName) {
    MyTextEdit::documentName = documentName;
}
