#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "online_synchronizer.h"
#include "shared_editor.h"
#include <QDebug>
#include <QFontDialog>
#include <QErrorMessage>
#include <QFontComboBox>
#include <QComboBox>
#include <QLabel>
#include <QUndoStack>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    this->toDelete = 0;
    ui->setupUi(this);
    ui->textEditShared->setAcceptRichText(true);

    // for blinking images
    //connect(&background_task,SIGNAL(tick_clock()),this,SLOT(redrawBlinkingImage()));
    //background_task.start();

    // for web request
    //OnlineSynchronizer* synk = new OnlineSynchronizer{};

    // setting up the cursor position to understand if text was inserted or deleted
    this->last_cursor_position = ui->textEditShared->textCursor().position();

    ui->textEditShared->installEventFilter(this);
    Shared_editor::getInstance().initString(ui->textEditShared->toHtml());

    // adding comboboxes for font type and size
    auto comboFont = new QFontComboBox(ui->mainToolBar);
    auto comboSize = new QComboBox(ui->mainToolBar);

    this->clipboard = QApplication::clipboard();

    QStringList tmp({"8","9","10","11","12","14","16","18","20","22","24","26","28","36","48","72"});
    this->fontSizes = tmp;
    comboSize->addItems(fontSizes);
    comboFont->setEditable(false);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[7], comboFont);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[8], comboSize);

    // setting up my connect event
    connect(ui->textEditShared,SIGNAL(selectionChanged()),this,SLOT(memorizeSelection()));

    connect(ui->actionCopy,SIGNAL(triggered()),ui->textEditShared,SLOT(copy()));
    connect(ui->actionCut,SIGNAL(triggered()),ui->textEditShared,SLOT(cut()));
    connect(ui->actionPaste,SIGNAL(triggered()),ui->textEditShared,SLOT(paste()));
    connect(comboFont,SIGNAL(activated(int)),this,SLOT(selectFont(int)));
    connect(comboSize,SIGNAL(activated(int)),this,SLOT(selectSize(int)));
    connect(ui->actionBold,SIGNAL(triggered()),this,SLOT(makeBold()));
    connect(ui->actionItalic,SIGNAL(triggered()),this,SLOT(makeItalic()));
    connect(ui->actionUnderlined,SIGNAL(triggered()),this,SLOT(makeUnderline()));
    connect(ui->actionAlignLeft,SIGNAL(triggered()),this,SLOT(alignLeft()));
    connect(ui->actionAlignCenter,SIGNAL(triggered()),this,SLOT(alignCenter()));
    connect(ui->actionAlignRight,SIGNAL(triggered()),this,SLOT(alignRight()));
    connect(ui->actionAlignJustify,SIGNAL(triggered()),this,SLOT(alignJustify()));

    connect(ui->textEditShared,&QTextEdit::textChanged,this,&MainWindow::textChanged);
    connect(ui->textEditShared,SIGNAL(cursorPositionChanged()),this,SLOT(checkFontProperty()));
    connect(this,SIGNAL(setComboSize(int)),comboSize,SLOT(setCurrentIndex(int)));
    connect(this,SIGNAL(setComboFont(QFont)),comboFont,SLOT(setCurrentFont(QFont)));
}

MainWindow::~MainWindow()
{
    background_task.cancel();
    background_task.wait();
    delete ui;
}

void MainWindow::exportPDF() {
    qDebug() << "exported pdf button pressed";
}

void MainWindow::alignLeft()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();
    auto textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignLeft);
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

void MainWindow::checkFontProperty()
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
