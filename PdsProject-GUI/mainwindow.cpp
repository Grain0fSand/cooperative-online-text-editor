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

    //connect(&background_task,SIGNAL(tick_clock()),this,SLOT(redrawBlinkingImage()));
    //background_task.start();

    OnlineSynchronizer* synk = new OnlineSynchronizer{};

    // setting up my connect event
    connect(ui->pushButtonFont,SIGNAL(clicked()),this,SLOT(selectFont()));
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

void MainWindow::redrawBlinkingImage() {
    qDebug() << "now it must redraw (show/hide) the images for blinking effect";
}

void MainWindow::selectFont()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    // TODO: left as it is, elia will fix it
    if (cursor.selectionEnd()&& cursor.selectionStart() != cursor.selectionEnd()){
        qDebug() << "end and begin are the same, so i change all the text";


        //QTextDocumentFragment sel = cursor.selection();

        cursor = textEdit->textCursor();
        QFont boldFont(textEdit->font());

        QFont initialFont = ui->textEditShared->font();
        qDebug() << initialFont.toString();

        bool ok;
        QFont font = QFontDialog::getFont(
                        &ok, initialFont, this);
        if (ok) {
            qDebug() << font.toString();
            ui->textEditShared->setFont(font);
            qDebug() << ui->textEditShared->font().toString();

            QTextCharFormat format;
            format.setFont(boldFont);
            cursor.setBlockCharFormat(format);
        }
    }

}
