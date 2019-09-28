#include "mytextedit.h"
#include "mainwindow.h"
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
    this->textColorsList.emplace_back("#61E761");
    this->textColorsList.emplace_back("#61E761");
    this->textColorsList.emplace_back("#61E761");
    this->textColorsList.emplace_back("#61E761");
    this->textColorsList.emplace_back("#4C331C");
    this->textColorsList.emplace_back("#4C331C");
    this->textColorsList.emplace_back("#4C331C");
    this->textColorsList.emplace_back("#4C331C");
    this->textColorsList.emplace_back("#EFB66F");
    this->textColorsList.emplace_back("#EFB66F");
    this->textColorsList.emplace_back("#EFB66F");
    this->textColorsList.emplace_back("#EFB66F");
    this->textColorsList.emplace_back("#D20E38");
    this->textColorsList.emplace_back("#D20E38");
    this->textColorsList.emplace_back("#D20E38");
    this->textColorsList.emplace_back("#D20E38");

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
                    this->hiddenCursor->setPosition(all_pos[ptr_start]);
                case TextFormatting:  //no break, insertion does formatting too
                {
                    this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor, ptr_end + 1 - ptr_start);
                    QTextCharFormat format;

                    action.getBold() ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
                    format.setFontItalic(action.getItalic());
                    format.setFontUnderline(action.getUnderlined());
                    format.setFontFamily(this->fontFamilies.at(action.getComboFontIndex()));
                    format.setFontPointSize(this->fontSizes.at(action.getComboFontIndex()).toInt());
                    this->hiddenCursor->setCharFormat(format);
                    break;
                }
                case Deletion:
                    this->hiddenCursor->movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,ptr_end + 1 - ptr_start);
                    this->hiddenCursor->removeSelectedText();
                    break;
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
                    this->hiddenCursor->setBlockFormat(format);
                    break;
                }
            }
            ++ptr_end;
        }
        MainWindow::getInstance().checkTextProperty();
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
            QColor color(this->textColorsList.at(n));
            QTextCharFormat format;
            format.setBackground(QBrush(color));
            if((color.red()*299 + color.green()*587 + color.blue()*114)/1000 < 123)
                format.setForeground(QBrush(QColor(Qt::white)));
            this->hiddenCursor->mergeCharFormat(format);
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
