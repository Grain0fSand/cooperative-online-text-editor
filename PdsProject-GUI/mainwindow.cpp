#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->textEditShared->setAcceptRichText(true);


    // setting up my connect event
    connect(ui->pushButtonFont,SIGNAL(clicked()),this,SLOT(selectFont()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exportPDF() {
    qDebug() << "exported pdf button pressed";
}

void MainWindow::selectFont()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    // TODO: left as it is, elia will fix it
    if (cursor.selectionStart() == cursor.selectionEnd()){
        qDebug() << 'end and begin are the same, so i change all the text';

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

            //QTextCharFormat format;
            //format.setFont(boldFont);
            //cursor.setBlockCharFormat(format);
        }
    }

}
