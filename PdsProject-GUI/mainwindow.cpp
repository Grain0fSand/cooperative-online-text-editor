#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "online_synchronizer.h"
#include "shared_editor.h"
#include <QDebug>
#include <QFontDialog>
#include <QErrorMessage>

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

    // setting up my connect event
    connect(ui->textEditShared,SIGNAL(selectionChanged()),this,SLOT(memorizeSelection()));
    connect(ui->pushButtonAlignLeft,SIGNAL(clicked()),this,SLOT(alignLeft()));
    connect(ui->pushButtonAlignCenter,SIGNAL(clicked()),this,SLOT(alignCenter()));
    connect(ui->pushButtonAlignRight,SIGNAL(clicked()),this,SLOT(alignRight()));
    connect(ui->pushButtonBold,SIGNAL(clicked()),this,SLOT(makeBold()));
    connect(ui->pushButtonFont,SIGNAL(clicked()),this,SLOT(selectFont()));
    connect(ui->pushButtonItalic,SIGNAL(clicked()),this,SLOT(makeItalic()));
    connect(ui->pushButtonUnderlined,SIGNAL(clicked()),this,SLOT(makeUnderline()));
    connect(ui->textEditShared,&QTextEdit::textChanged,this,&MainWindow::textChanged);
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

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_bold = !font.bold();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        auto text = cursor.selectedText();
        QString pre = "<p align=\"left\">";
        QString post = "</p>";

        Shared_editor::getInstance().addString(cursor.selectionStart(),pre);
        Shared_editor::getInstance().addString(cursor.selectionEnd(),post);

        auto rendered = Shared_editor::getInstance().toString();
        qDebug() << rendered;


        //cursor.insertHtml(pre + text + post);
        ui->textEditShared->setHtml(rendered);
    }
}

void MainWindow::alignCenter()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_bold = !font.bold();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        auto text = cursor.selectedText();

        QString pre = "<p align=\"center\">";
        QString post = "</p>";

        Shared_editor::getInstance().addString(cursor.selectionStart(),pre);
        Shared_editor::getInstance().addString(cursor.selectionEnd(),post);

        cursor.insertHtml(pre + text + post);
    }
}

void MainWindow::alignRight()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_bold = !font.bold();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        auto text = cursor.selectedText();
        QString pre = "<p align=\"right\">";
        QString post = "</p>";

        Shared_editor::getInstance().addString(cursor.selectionStart(),pre);
        Shared_editor::getInstance().addString(cursor.selectionEnd(),post);

        cursor.insertHtml(pre + text + post);
    }
}

void MainWindow::memorizeSelection()
{
    auto cursor = ui->textEditShared->textCursor();
    this->toDelete = cursor.selectionStart() - cursor.selectionEnd();
}

// TODO: refactory for shared_editor.h
void MainWindow::makeBold()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_bold = !font.bold();

    if (cursor.selectionStart() != cursor.selectionEnd()){



        //font.setBold(is_bold);

        //format.setFont(font);

        //cursor.setCharFormat(format);
        //int pos = cursor.selectionEnd();
        //cursor.setPosition(pos);

        //format.setFont(initial_font);
        //cursor.setCharFormat(format);
        //textEdit->setTextCursor(cursor);
    } else {
        initial_font.setBold(is_bold);
        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }

    qDebug() << ui->textEditShared->toHtml();
}

void MainWindow::redrawBlinkingImage() {
    qDebug() << "now it must redraw (show/hide) the images for blinking effect";
}

// TODO: refactory for shared_editor.h
void MainWindow::makeItalic()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_italic = !font.italic();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        font.setItalic(is_italic);

        format.setFont(font);

        cursor.setCharFormat(format);
        int pos = cursor.selectionEnd();
        cursor.setPosition(pos);

        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    } else {
        font.setItalic(is_italic);
        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
}


// TODO: refactory for shared_editor.h
void MainWindow::makeUnderline()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_underlined = !font.underline();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        font.setUnderline(is_underlined);

        format.setFont(font);

        cursor.setCharFormat(format);
        int pos = cursor.selectionEnd();
        cursor.setPosition(pos);

        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    } else {
        initial_font.setUnderline(is_underlined);
        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
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

    qDebug() << Shared_editor::getInstance().toString();

}


// TODO: refactory for shared_editor.h
void MainWindow::selectFont()
{
    auto initial_font = ui->textEditShared->font();
    bool ok;
    QFont font = QFontDialog::getFont(&ok, initial_font, this);

    auto text_cursor = ui->textEditShared->textCursor();
    auto format = text_cursor.charFormat();
    format.setFont(font);

    text_cursor.setCharFormat(format);
    ui->textEditShared->setTextCursor(text_cursor);

    qDebug() << ui->textEditShared->toHtml();

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
