#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "online_synchronizer.h"
#include "shared_editor.h"
#include "usertag.h"
#include <QDebug>
#include <QFontDialog>
#include <QErrorMessage>
#include <QFontComboBox>
#include <QComboBox>
#include <QLabel>
#include <QUndoStack>
#include <QClipboard>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    this->toDelete = 0;
    ui->setupUi(this);
    ui->textEditShared->setAcceptRichText(true);

    /*
    UserTag testTag;
    auto widget = testTag.createTag();
    testTag.chooseUserLogo();
    widget->setParent(ui->usersAreaWidgetContents);
    */

    // for blinking images
    //connect(&background_task,SIGNAL(tick_clock()),this,SLOT(redrawBlinkingImage()));
    //background_task.start();

    // for web request
    //OnlineSynchronizer* synk = new OnlineSynchronizer{};

    // setting up the cursor position to understand if text was inserted or deleted
    this->last_cursor_position = ui->textEditShared->textCursor().position();

    ui->textEditShared->installEventFilter(this);
    Shared_editor::getInstance().initString(ui->textEditShared->toHtml());
    ui->textEditShared->setCurrentFont(QFont("Calibri",11,-1,false));

    this->clipboard = QApplication::clipboard();

    // adding comboboxes for font type and size
    auto comboFont = new QFontComboBox(ui->mainToolBar);
    auto comboSize = new QComboBox(ui->mainToolBar);

    this->fontSizes << "8" << "9" << "10" << "11" << "12" <<
                       "14" << "16" << "18" << "20" << "22" <<
                       "24" << "26" << "28" << "36" << "48" << "72";
    comboSize->addItems(fontSizes);
    comboSize->setCurrentText(QString::number(ui->textEditShared->currentFont().pointSize()));
    comboFont->setCurrentFont(ui->textEditShared->currentFont());
    comboFont->setEditable(false);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[7], comboFont);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[8], comboSize);

    // setting up my connect event
    connect(ui->textEditShared,&QTextEdit::selectionChanged,this,&MainWindow::memorizeSelection);

    connect(ui->actionCopy,&QAction::triggered,ui->textEditShared,&QTextEdit::copy);
    connect(ui->actionCut,&QAction::triggered,ui->textEditShared,&QTextEdit::cut);
    connect(ui->actionPaste,&QAction::triggered,ui->textEditShared,&QTextEdit::paste);
    connect(comboFont,SIGNAL(activated(int)),this,SLOT(selectFont(int)));
    connect(comboSize,SIGNAL(activated(int)),this,SLOT(selectSize(int)));
    connect(ui->actionBold,&QAction::triggered,this,&MainWindow::makeBold);
    connect(ui->actionItalic,&QAction::triggered,this,&MainWindow::makeItalic);
    connect(ui->actionUnderlined,&QAction::triggered,this,&MainWindow::makeUnderline);
    connect(ui->actionAlignLeft,&QAction::triggered,this,&MainWindow::alignLeft);
    connect(ui->actionAlignCenter,&QAction::triggered,this,&MainWindow::alignCenter);
    connect(ui->actionAlignRight,&QAction::triggered,this,&MainWindow::alignRight);
    connect(ui->actionAlignJustify,&QAction::triggered,this,&MainWindow::alignJustify);

    //connect(ui->textEditShared,&QTextEdit::textChanged,this,&MainWindow::textChanged);
    connect(ui->textEditShared,&QTextEdit::cursorPositionChanged,this,&MainWindow::checkTextProperty);
    connect(this,&MainWindow::setComboSize,comboSize,&QComboBox::setCurrentIndex);
    connect(this,&MainWindow::setComboFont,comboFont,&QFontComboBox::setCurrentFont);
    connect(ui->actionExport_to_PDF,&QAction::triggered,this,&MainWindow::exportPDF);
}

MainWindow::~MainWindow()
{
    background_task.cancel();
    background_task.wait();
    delete ui;
}

void MainWindow::exportPDF()
{
    QString file_path = QFileDialog::getSaveFileName(this,"Export to PDF..","","Pdf file (*.pdf);;All Files (*)");

    if(file_path!="") {
        QPrinter* printer = new QPrinter(QPrinter::PrinterResolution);
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(file_path);
        printer->setPaperSize(QPrinter::A4);
        ui->textEditShared->print(printer);
        QString file_name = file_path.split("/").last();
        if (printer->printerState()<2) {
            QMessageBox::information(this,"Export to PDF","File "+file_name+" exported successfully.");
        }
        else {
            QMessageBox::information(this,"Error","Error exporting "+file_name+".");
        }
    }
}

void MainWindow::alignLeft()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignLeft);
    ui->actionAlignLeft->setChecked(true);
    ui->actionAlignCenter->setChecked(false);
    ui->actionAlignRight->setChecked(false);
    ui->actionAlignJustify->setChecked(false);

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
}

void MainWindow::alignCenter()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignCenter);
    ui->actionAlignLeft->setChecked(false);
    ui->actionAlignCenter->setChecked(true);
    ui->actionAlignRight->setChecked(false);
    ui->actionAlignJustify->setChecked(false);

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);

}

void MainWindow::alignRight()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignRight);
    ui->actionAlignLeft->setChecked(false);
    ui->actionAlignCenter->setChecked(false);
    ui->actionAlignRight->setChecked(true);
    ui->actionAlignJustify->setChecked(false);

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
}

void MainWindow::alignJustify()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignJustify);
    ui->actionAlignLeft->setChecked(false);
    ui->actionAlignCenter->setChecked(false);
    ui->actionAlignRight->setChecked(false);
    ui->actionAlignJustify->setChecked(true);

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
}

void MainWindow::memorizeSelection()
{
    auto cursor = ui->textEditShared->textCursor();
    this->toDelete = cursor.selectionStart() - cursor.selectionEnd();
}

void MainWindow::redrawBlinkingImage() {
    qDebug() << "now it must redraw (show/hide) the images for blinking effect";
}

// TODO: refactory for shared_editor.h
void MainWindow::makeBold()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto format = cursor.charFormat();
    auto font = format.font();

    auto is_bold = ui->actionBold->isChecked();
    /*
    if(cursor.hasSelection()) {
        auto text = cursor.selectedText();
        cursor.insertHtml("<b>" + text + "</b>");
    }
    else {
        (is_bold) ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
        cursor.mergeCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
    */

    ui->actionBold->setChecked(is_bold);
    (is_bold) ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    qDebug() << textEdit->toHtml();
}

// TODO: refactory for shared_editor.h
void MainWindow::makeItalic()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto format = cursor.charFormat();
    auto font = format.font();

    auto is_italic = ui->actionItalic->isChecked();

    ui->actionItalic->setChecked(is_italic);
    /*
    if(cursor.hasSelection()) {
        auto text = cursor.selectedText();
        cursor.insertHtml("<i>" + text + "</i>");
    }
    else {
        format.setFontItalic(is_italic);
        cursor.mergeCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
    */
    ui->actionItalic->setChecked(is_italic);
    format.setFontItalic(is_italic);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    qDebug() << textEdit->toHtml();
}


// TODO: refactory for shared_editor.h
void MainWindow::makeUnderline()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto format = cursor.charFormat();
    auto font = format.font();

    auto is_underlined = ui->actionUnderlined->isChecked();
    /*
    if(cursor.hasSelection()) {
        auto text = cursor.selectedText();
        cursor.insertHtml("<u>" + text + "</u>");
    }
    else {
        format.setFontUnderline(is_underlined);
        cursor.mergeCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
    */


    ui->actionUnderlined->setChecked(is_underlined);
    format.setFontUnderline(is_underlined);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    qDebug() << textEdit->toHtml();
}

// TODO: refactory delete for shared_editor.h
void MainWindow::textChanged() {
    int previus_position = this->last_cursor_position;
    this->last_cursor_position = ui->textEditShared->textCursor().position();

    bool is_text_deleted = (this->last_cursor_position-previus_position) < 0;

    if (is_text_deleted){
        Shared_editor::getInstance().removeString(this->last_cursor_position,toDelete);
        qDebug() << "the text was deleted";
    } else {
        int pos = this->last_cursor_position-1;
        auto charapter = ui->textEditShared->toPlainText().at(pos);
        Shared_editor::getInstance().addCharapter(pos,charapter);
        qDebug() << "the text was inserted";
    }

    //qDebug() << Shared_editor::getInstance().toString();

}


// TODO: refactory for shared_editor.h
void MainWindow::selectFont(int fontIndex)
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto format = cursor.charFormat();

    QFontDatabase fontDb;
    QString fontFamily = fontDb.families()[fontIndex];

    format.setFontFamily(fontFamily);
    cursor.setCharFormat(format);
    textEdit->setTextCursor(cursor);
}

void MainWindow::selectSize(int sizeIndex)
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto format = cursor.charFormat();

    int size = this->fontSizes[sizeIndex].toInt();

    format.setFontPointSize(size);
    cursor.setCharFormat(format);
    textEdit->setTextCursor(cursor);
}

void MainWindow::checkTextProperty()
{
    auto text_cursor = ui->textEditShared->textCursor();

    bool cursor_visible = !text_cursor.hasSelection();
    ui->textEditShared->setCursorWidth(cursor_visible);

    auto format = text_cursor.charFormat();
    auto font = format.font();

    ui->actionBold->setChecked(font.bold());
    ui->actionItalic->setChecked(font.italic());
    ui->actionUnderlined->setChecked(font.underline());

    QString fontSize = QString::number(font.pointSize());
    int sizeIndex = fontSizes.indexOf(fontSize);
    setComboSize(sizeIndex);
    setComboFont(font);

    ui->actionAlignLeft->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignLeft));
    ui->actionAlignCenter->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignCenter));
    ui->actionAlignRight->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignRight));
    ui->actionAlignJustify->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignJustify));

    ui->actionCopy->setEnabled(ui->textEditShared->textCursor().hasSelection());
    ui->actionCut->setEnabled(ui->textEditShared->textCursor().hasSelection());

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // filter to remove undo and redo

        if (keyEvent->matches(QKeySequence::Undo))
        {
            qDebug() << "Undo pressed" << keyEvent->key();
            return true;
        }
        if (keyEvent->matches(QKeySequence::Redo))
        {
            qDebug() << "Redo pressed" << keyEvent->key();
            return true;
        }
        return false;
    } else {
        return false;
    }
}
