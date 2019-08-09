#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "online_synchronizer.h"
#include <QDebug>
#include <QFontDialog>
#include <QErrorMessage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    ui->setupUi(this);
    ui->textEditShared->setAcceptRichText(true);

    // for blinking images
    //connect(&background_task,SIGNAL(tick_clock()),this,SLOT(redrawBlinkingImage()));
    //background_task.start();

    // for web request
    //OnlineSynchronizer* synk = new OnlineSynchronizer{};

    // setting up the cursor position to understand if text was inserted or deleted
    this->last_cursor_position = ui->textEditShared->textCursor().position();

    // setting up my connect event
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
        cursor.insertHtml("<p align=\"left\">" + text + "</p>");
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
        cursor.insertHtml("<p align=\"center\">" + text + "</p>");
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
        cursor.insertHtml("<p align=\"right\">" + text + "</p>");
    }
}

void MainWindow::makeBold()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto format = cursor.charFormat();
    auto font = format.font();
    auto initial_font = font;

    auto is_bold = !font.bold();

    if (cursor.selectionStart() != cursor.selectionEnd()){
        font.setBold(is_bold);

        format.setFont(font);

        cursor.setCharFormat(format);
        int pos = cursor.selectionEnd();
        cursor.setPosition(pos);

        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    } else {
        initial_font.setBold(is_bold);
        format.setFont(initial_font);
        cursor.setCharFormat(format);
        textEdit->setTextCursor(cursor);
    }
}

void MainWindow::redrawBlinkingImage() {
    qDebug() << "now it must redraw (show/hide) the images for blinking effect";
}

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

void MainWindow::textChanged() {
    int previus_position = this->last_cursor_position;
    this->last_cursor_position = ui->textEditShared->textCursor().position();

    bool is_text_deleted = (this->last_cursor_position-previus_position) < 0;

    if (is_text_deleted){
        qDebug() << "the text was deleted";
    } else {
        qDebug() << "the text was inserted";
    }


    int pos = this->last_cursor_position-1;

    if (pos >= 0){
        auto charapter = ui->textEditShared->toPlainText().at(pos);

        qDebug() << "the previus char of the deleted is: " << charapter;
        //auto text = ui->textEditShared;
    } else {
        qDebug() << "all the text is deleted";
    }


}


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
