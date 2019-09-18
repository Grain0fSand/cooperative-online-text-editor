#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../web_requests/online_synchronizer.h"
#include "../utility/shared_editor.h"
#include "mytextedit.h"
#include "usertag.h"
#include "../web_requests/smtpclient.h"
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
#include <QGroupBox>
#include <QInputDialog>
#include <QPainter>
#include <QPushButton>
#include <QStringListModel>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    ui->setupUi(this);
    ui->textEditShared->setAcceptRichText(false); //this needs to be false to avoid pasting formatted text with CTRL+V

    // for web request
    //OnlineSynchronizer* synk = new OnlineSynchronizer{};

    ui->textEditShared->installEventFilter(this);
    Shared_editor::getInstance().initString(ui->textEditShared->toHtml());

    this->clipboard = QApplication::clipboard();

    ui->sideLayout->layout()->setAlignment(Qt::AlignTop);

    auto comboSize = new QComboBox(ui->mainToolBar);
    auto comboFamily = new QComboBox(ui->mainToolBar);
    setupFontComboBoxes(comboSize, comboFamily);

    setupStatusBar();

    // setting up my connect event
    connect(ui->actionCopy,&QAction::triggered,ui->textEditShared,&QTextEdit::copy);
    connect(ui->actionCut,&QAction::triggered,ui->textEditShared,&QTextEdit::cut);
    connect(ui->actionPaste,&QAction::triggered,ui->textEditShared,&QTextEdit::paste);
    connect(comboFamily,SIGNAL(activated(int)),this,SLOT(selectFont(int)));
    connect(comboSize,SIGNAL(activated(int)),this,SLOT(selectSize(int)));
    connect(ui->actionBold,&QAction::triggered,this,&MainWindow::makeBold);
    connect(ui->actionItalic,&QAction::triggered,this,&MainWindow::makeItalic);
    connect(ui->actionUnderlined,&QAction::triggered,this,&MainWindow::makeUnderline);
    connect(ui->actionAlignLeft,&QAction::triggered,this,&MainWindow::alignLeft);
    connect(ui->actionAlignCenter,&QAction::triggered,this,&MainWindow::alignCenter);
    connect(ui->actionAlignRight,&QAction::triggered,this,&MainWindow::alignRight);
    connect(ui->actionAlignJustify,&QAction::triggered,this,&MainWindow::alignJustify);

    connect(ui->textEditShared->document(),&QTextDocument::contentsChange,this,&MainWindow::textChanged);
    connect(ui->textEditShared,&QTextEdit::cursorPositionChanged,this,&MainWindow::checkTextProperty);
    connect(this,&MainWindow::setComboSize,comboSize,&QComboBox::setCurrentIndex);
    connect(this,&MainWindow::setComboFont,comboFamily,&QComboBox::setCurrentIndex);
    connect(ui->actionExport_to_PDF,&QAction::triggered,this,&MainWindow::exportPDF);
    connect(ui->actionInvite,&QAction::triggered,this,&MainWindow::reqInvitationEmailAddress);
    connect(ui->actionTestCursor,&QAction::triggered,this,&MainWindow::insertRemoteCursor); //only for test
    connect(ui->actionTestTag,&QAction::triggered,this,&MainWindow::addUserTag);    //only for test
    connect(ui->actionTestDisconnect,&QAction::triggered,this,&MainWindow::disableEditor); //only for test
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
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(file_path);
        printer.setPaperSize(QPrinter::A4);
        ui->textEditShared->print(&printer);
        QString file_name = file_path.split("/").last();
        if (printer.printerState()<2) {
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

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),AlignLeft); //selectionStart + 1 is needed by cdrt
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

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),AlignCenter);
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

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),AlignRight);
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

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),AlignJustify);
}

void MainWindow::makeBold()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto is_bold = ui->actionBold->isChecked();

    ui->actionBold->setChecked(is_bold);
    QTextCharFormat format;
    (is_bold) ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),is_bold,Bold);
}

void MainWindow::makeItalic()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    bool is_italic = ui->actionItalic->isChecked();

    ui->actionItalic->setChecked(is_italic);
    QTextCharFormat format;
    format.setFontItalic(is_italic);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),is_italic,Italic);
}

void MainWindow::makeUnderline()
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto is_underlined = ui->actionUnderlined->isChecked();

    ui->actionUnderlined->setChecked(is_underlined);
    QTextCharFormat format;
    format.setFontUnderline(is_underlined);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),is_underlined,Underlined);
}

//TODO: different methods for rich text and plain text
void MainWindow::textChanged(int pos, int nDel, int nIns) {

    if(nDel==0) {  //insertion
        QString str = ui->textEditShared->document()->toPlainText().mid(pos, nIns);
//        for(int i=0; i<nIns; i++) {
//            str += ui->textEditShared->document()->characterAt(pos+i);
//        }
//        qDebug() << str;
        ui->textEditShared->addAction(pos, nIns, str);
    } else if (nIns==0) { //deletion
        ui->textEditShared->addAction(pos + 1, nDel); //pos + 1 is needed by cdrt
    } else {
        //nothing to do
    }
}

void MainWindow::selectFont(int familyIndex)
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    QString family = ui->textEditShared->getFontFamilies()[familyIndex];
    QTextCharFormat format;
    format.setFontFamily(family);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),familyIndex,FontFamily); //selectionStart + 1 is needed by cdrt
}

void MainWindow::selectSize(int sizeIndex)
{
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    int size = ui->textEditShared->getFontSizes()[sizeIndex].toInt();
    QTextCharFormat format;
    format.setFontPointSize(size);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    ui->textEditShared->addAction(cursor.selectionStart() + 1,cursor.selectedText().length(),sizeIndex,FontSize);
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
    int sizeIndex = ui->textEditShared->getFontSizes().indexOf(fontSize);
    int familyIndex = ui->textEditShared->getFontFamilies().indexOf(font.family());
    setComboSize(sizeIndex);
    setComboFont(familyIndex);

    ui->actionAlignLeft->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignLeft));
    ui->actionAlignCenter->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignCenter));
    ui->actionAlignRight->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignRight));
    ui->actionAlignJustify->setChecked(text_cursor.blockFormat().alignment().testFlag(Qt::AlignJustify));

    ui->actionCopy->setEnabled(ui->textEditShared->textCursor().hasSelection());
    ui->actionCut->setEnabled(ui->textEditShared->textCursor().hasSelection());

    ui->statusBar->findChild<QLabel*>("charCount")->setText("Chars: "+QString::number(ui->textEditShared->document()->characterCount()));
    ui->statusBar->findChild<QLabel*>("lineCount")->setText("Lines: "+QString::number(ui->textEditShared->document()->lineCount()));
    ui->statusBar->findChild<QLabel*>("cursorPos")->setText("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    ui->statusBar->findChild<QLabel*>("cursorColumn")->setText("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    ui->statusBar->findChild<QLabel*>("cursorSelectionCount")->setText("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));
}

void MainWindow::insertRemoteCursor() {
    ui->textEditShared->createCursor(ui->textEditShared->textCursor().position(),"prova1",Qt::red);
    ui->textEditShared->repaint();
}

void MainWindow::reqInvitationEmailAddress()
{
    QString message = "";
    QInputDialog dialog(this);
    dialog.setLabelText("Insert destination Email address:");
    dialog.setModal(true);
    while(true) {
        bool error = false;
        if(dialog.exec()==QDialog::Accepted) {
            if(dialog.textValue()=="") {
                error=true;
                message = "You left the field empty!\nEnter a valid Email address";
            }
            else {
                QRegExp mailRegex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
                mailRegex.setCaseSensitivity(Qt::CaseInsensitive);
                //mailRegex.setPatternSyntax(QRegExp::RegExp);
                if(!mailRegex.exactMatch(dialog.textValue())) {
                    error=true;
                    message = "Email address not valid\n"
                              "Insert a valid one";
                }
            }

            if(error) {
                QMessageBox advice(this);
                advice.setText(message);
                advice.setIcon(QMessageBox::Critical);
                advice.exec();
            }
            else {
                sendInvitationEmail(dialog.textValue());
                break;
            }
        }
        else break;
    }
}

void MainWindow::addUserTag()
{
    auto tag = new UserTag();
    this->usersList.push_back(tag);

    ui->listOnlineUsers->setItemDelegate(tag);
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole + 1, "Username");
    item->setData(Qt::UserRole + 2, "Online");
    item->setData(Qt::UserRole + 3, QPixmap("://PdsProject.app/Contents/Resources/avatars/avatar.png"));
    item->setData(Qt::UserRole + 4, QPixmap("://PdsProject.app/Contents/Resources/img/greenLed.png"));
    item->setData(Qt::UserRole + 5, QColor(Qt::red));
    ui->listOnlineUsers->addItem(item);
}

void MainWindow::disableEditor()
{
    ui->textEditShared->setEnabled(!ui->textEditShared->isEnabled());
    ui->statusBar->setEnabled(!ui->statusBar->isEnabled());
    ui->actionCopy->setEnabled(!ui->actionCopy->isEnabled());
    ui->actionCut->setEnabled(!ui->actionCut->isEnabled());
    auto comboBoxes = ui->mainToolBar->findChildren<QComboBox*>();
    comboBoxes[0]->setEnabled(!comboBoxes[0]->isEnabled());
    comboBoxes[1]->setEnabled(!comboBoxes[1]->isEnabled());
    ui->actionPaste->setEnabled(!ui->actionPaste->isEnabled());
    ui->actionBold->setEnabled(!ui->actionBold->isEnabled());
    ui->actionItalic->setEnabled(!ui->actionItalic->isEnabled());
    ui->actionUnderlined->setEnabled(!ui->actionUnderlined->isEnabled());
    ui->actionAlignLeft->setEnabled(!ui->actionAlignLeft->isEnabled());
    ui->actionAlignCenter->setEnabled(!ui->actionAlignCenter->isEnabled());
    ui->actionAlignRight->setEnabled(!ui->actionAlignRight->isEnabled());
    ui->actionAlignJustify->setEnabled(!ui->actionAlignJustify->isEnabled());
    ui->listOnlineUsers->setEnabled(!ui->listOnlineUsers->isEnabled());
    ui->listOfflineUsers->setEnabled(!ui->listOfflineUsers->isEnabled());
    ui->onlineRollButton->setEnabled(!ui->onlineRollButton->isEnabled());
    ui->offlineRollButton->setEnabled(!ui->offlineRollButton->isEnabled());
}

void MainWindow::setupFontComboBoxes(QComboBox* comboSize, QComboBox* comboFamily)
{
    auto font = comboSize->font();
    font.setPointSize(13);
    comboSize->setFont(font);
    comboFamily->setFont(font);

    comboSize->addItems(ui->textEditShared->getFontSizes());
    comboSize->setCurrentText(QString::number(ui->textEditShared->currentFont().pointSize()));

    comboFamily->addItems(ui->textEditShared->getFontFamilies());
    QIcon icon("://PdsProject.app/Contents/Resources/img/font_icon.png");
    for(int i=0; i<comboFamily->count(); i++) {
        comboFamily->setItemIcon(i, icon);
    }
    comboFamily->setCurrentText(ui->textEditShared->currentFont().family());

    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[7], comboFamily);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[8], comboSize);
}

void MainWindow::setupStatusBar()
{
    ui->textEditShared->setDocumentTitle("file_name.txt");
    QLabel *filename = new QLabel("Document: "+ui->textEditShared->documentTitle());
    filename->setObjectName("filename");
    QLabel *charCount = new QLabel("Chars: "+QString::number(ui->textEditShared->document()->characterCount()));
    charCount->setObjectName("charCount");
    QLabel *lineCount = new QLabel("Lines: "+QString::number(ui->textEditShared->document()->lineCount()));
    lineCount->setObjectName("lineCount");
    QLabel *cursorPos = new QLabel("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    cursorPos->setObjectName("cursorPos");
    QLabel *cursorColumn = new QLabel("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    cursorColumn->setObjectName("cursorColumn");
    QLabel *cursorSelectionCount = new QLabel("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));
    cursorSelectionCount->setObjectName("cursorSelectionCount");

    ui->statusBar->addWidget(filename, 2);
    ui->statusBar->addWidget(charCount, 0);
    ui->statusBar->addWidget(lineCount, 1);
    ui->statusBar->addWidget(cursorPos, 0);
    ui->statusBar->addWidget(cursorColumn, 0);
    ui->statusBar->addWidget(cursorSelectionCount, 2);
}

void MainWindow::sendInvitationEmail(QString destEmailAddress)
{
    SmtpClient smtp;
    smtp.setUser("simulpad.texteditor@gmail.com");
    smtp.setPassword("progettomalnati");
    smtp.setMailSender("simulpad.texteditor@gmail.com","SimulPad");
    smtp.setMailDestination(destEmailAddress,"no_name");
    smtp.setMailSubject("Someone invites to collaborate on SimulPad");
    smtp.prepareMailText("Here goes the URI");

    if(smtp.connectToHost()) {
        if(smtp.login()) {
            if(smtp.sendMail()) {
                QMessageBox::information(this,"Succeed","Your invitation was sent to "+destEmailAddress);
            }
        }
    }
    smtp.quit();
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

void MainWindow::on_onlineRollButton_clicked()
{
    ui->listOnlineUsers->setVisible(!ui->listOnlineUsers->isVisible());
    if(ui->listOnlineUsers->isVisible()) {
        ui->onlineRollButton->setIcon(QIcon("://PdsProject.app/Contents/Resources/img/arrow_up.png"));
    }
    else {
        ui->onlineRollButton->setIcon(QIcon("://PdsProject.app/Contents/Resources/img/arrow_down.png"));
    }
}
void MainWindow::on_offlineRollButton_clicked()
{
    ui->listOfflineUsers->setVisible(!ui->listOfflineUsers->isVisible());
    if(ui->listOfflineUsers->isVisible()) {
        ui->offlineRollButton->setIcon(QIcon("://PdsProject.app/Contents/Resources/img/arrow_up.png"));
    }
    else {
        ui->offlineRollButton->setIcon(QIcon("://PdsProject.app/Contents/Resources/img/arrow_down.png"));
    }
}

void MainWindow::on_actionTestActions_triggered()
{
    Action action;

//    action.setActionType(Insertion);
//    action.setCursorPos(0);
//    action.setNumChars(4);
//    action.setChars("ciao");


/*    action.setActionType(Deletion);
    action.setCursorPos(10);
    action.setNumChars(6);
*/

    action.setActionType(TextFormatting);
    action.setCursorPos(10);
    action.setNumChars(6);
    action.setTextFormatType(Bold);
    action.setTextFormatBoolean(true);


/*  action.setActionType(BlockFormatting);
    action.setCursorPos(10);
    action.setNumChars(30);
    action.setBlockFormatType(AlignCenter);
*/

    ui->textEditShared->toDoList.push_front(action);
    ui->textEditShared->doReceivedActions();
}

//TODO: clicking on bold/cursive/etc. with nothing highlighted sends an action but it shouldn't
//TODO: block formatting doesn't result in sending an action but it should
//TODO: fix error caused by action changes