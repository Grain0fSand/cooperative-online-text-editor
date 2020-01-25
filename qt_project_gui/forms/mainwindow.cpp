#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "../utility/shared_editor.h"
#include "mytextedit.h"
#include "usertag.h"
#include "../web_requests/smtpclient.h"
#include "../data_structure/crdt.h"
#include <random>
#include <QDebug>
#include <QFontDialog>
#include <QFontComboBox>
#include <QComboBox>
#include <QLabel>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QPushButton>
#include <QStringListModel>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    ui->setupUi(this);

    SessionData::accessToSessionData().youWannaLogin = false;
    SessionData::accessToSessionData().mainWindowPointer = this;
    SessionData::accessToSessionData().myTextEditPointer = ui->textEditShared;

    auto overlayWidget = new OverlayWidget(ui->docFrame);
    QRect frameRect = ui->docFrame->geometry();
    frameRect.setHeight(ui->centralWidget->geometry().height());
    overlayWidget->setGeometry(frameRect);
    overlayWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

    Crdt::getInstance().init(std::stoi(SessionData::accessToSessionData().userId));
    query = new OnlineQuery{SessionData::accessToSessionData().docId, SessionData::accessToSessionData().token, this};
    query->start();

    ui->myUsername->setText(QString::fromStdString(SessionData::accessToSessionData().username));
    ui->myAvatar->setPixmap(SessionData::accessToSessionData().avatar);
    ui->textEditShared->setAcceptRichText(false); //this needs to be false to avoid pasting formatted text with CTRL+V
    ui->textEditShared->installEventFilter(this);
    ui->textEditShared->setDocumentName(QString::fromStdString(SessionData::accessToSessionData().docName));
    Shared_editor::getInstance().initString(ui->textEditShared->toHtml());

    ui->sideLayout->layout()->setAlignment(Qt::AlignTop);

    auto comboSize = new QComboBox(ui->mainToolBar);
    auto comboFamily = new QComboBox(ui->mainToolBar);
    setupFontComboBoxes(comboSize, comboFamily);
    setupStatusBar();

    populateUserTagList();

    // setting up my connect event
    connect(ui->actionCopy, &QAction::triggered, ui->textEditShared, &QTextEdit::copy);
    connect(ui->actionCut, &QAction::triggered, ui->textEditShared, &QTextEdit::cut);
    connect(ui->actionPaste, &QAction::triggered, ui->textEditShared, &QTextEdit::paste);
    connect(comboFamily, SIGNAL(activated(int)), this, SLOT(selectFont(int)));
    connect(comboSize, SIGNAL(activated(int)), this, SLOT(selectSize(int)));
    connect(ui->actionBold, &QAction::triggered, this, &MainWindow::makeBold);
    connect(ui->actionItalic, &QAction::triggered, this, &MainWindow::makeItalic);
    connect(ui->actionUnderlined, &QAction::triggered, this, &MainWindow::makeUnderline);
    connect(ui->actionAlignLeft, &QAction::triggered, this, &MainWindow::alignLeft);
    connect(ui->actionAlignCenter, &QAction::triggered, this, &MainWindow::alignCenter);
    connect(ui->actionAlignRight, &QAction::triggered, this, &MainWindow::alignRight);
    connect(ui->actionAlignJustify, &QAction::triggered, this, &MainWindow::alignJustify);
    connect(ui->textEditShared->document(), &QTextDocument::contentsChange, this, &MainWindow::textChanged);
    connect(ui->textEditShared, &QTextEdit::selectionChanged, ui->textEditShared,
            [&]() { ui->textEditShared->previousSelection = ui->textEditShared->textCursor().selectedText().count();
                if (ui->textEditShared->previousPreviousSelection != ui->textEditShared->previousSelection)
                    ui->textEditShared->colorText(ui->textEditShared->colorFeatureActive);
                ui->textEditShared->previousPreviousSelection = ui->textEditShared->previousSelection;
    });
    connect(ui->textEditShared, &QTextEdit::cursorPositionChanged, this, &MainWindow::checkTextProperty);
    connect(this, &MainWindow::setComboSize, comboSize, &QComboBox::setCurrentIndex);
    connect(this, &MainWindow::setComboFont, comboFamily, &QComboBox::setCurrentIndex);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitFromEditor);
    connect(ui->actionLogout, &QAction::triggered, this, &MainWindow::backToLogin);
    connect(ui->actionBack, &QAction::triggered, this, &MainWindow::backToPersonalPage);
    connect(ui->actionExport_to_PDF, &QAction::triggered, this, &MainWindow::exportPDF);
    connect(ui->actionInvite, &QAction::triggered, this, &MainWindow::reqInvitationEmailAddress);
    connect(ui->actionCursor, &QAction::toggled, ui->textEditShared, &MyTextEdit::showRemoteCursors);
    connect(ui->actionColor, &QAction::toggled, ui->textEditShared, &MyTextEdit::colorText);
    connect(&Crdt::getInstance(),&Crdt::needToResetLastCrdtId,query,&OnlineQuery::resetLastCrdtId);
}

MainWindow::~MainWindow()
{
    background_task.cancel();
    background_task.wait();
    delete ui;
}

void MainWindow::populateUserTagList()
{
    QString statusLabel;
    QPixmap led;

    ui->listOnlineUsers->clear();
    ui->listOfflineUsers->clear();

    ui->actionColor->setEnabled(SessionData::accessToSessionData().usersList.size() > 1);

    for (auto &tag : SessionData::accessToSessionData().usersList) {
        if (tag.getUserId() == std::stoi(SessionData::accessToSessionData().userId))
            continue;

        auto item = new QListWidgetItem();
        if (tag.getStatus()) {
            led.load(":/resources/greenLed.png");
            statusLabel = "Online";
            ui->listOnlineUsers->setItemDelegate(&tag);
            ui->listOnlineUsers->addItem(item);
        } else {
            led.load(":/resources/redLed.png");
            statusLabel = "Offline";
            ui->listOfflineUsers->setItemDelegate(&tag);
            ui->listOfflineUsers->addItem(item);
        }
        item->setData(Qt::UserRole + 1, tag.getUsername());
        item->setData(Qt::UserRole + 2, statusLabel);
        item->setData(Qt::UserRole + 3, tag.getAvatar());
        item->setData(Qt::UserRole + 4, led);
        item->setData(Qt::UserRole + 5, tag.getUserColor());
    }
}

void MainWindow::arrangeUserTagList(std::vector<exchangeable_data::user> remoteVector) {

    SessionData *sessionData = &SessionData::accessToSessionData();

    if (sessionData->onlineUsers.size() != remoteVector.size()) {   //number of user change

        ui->actionCursor->setEnabled(remoteVector.size() > 1);

        ui->textEditShared->clearRemoteCursorList();
        std::vector<UserTag>::iterator everytimeUser;
        std::vector<exchangeable_data::user>::iterator onlineUser;

        for(everytimeUser=sessionData->usersList.begin(); everytimeUser<sessionData->usersList.end(); everytimeUser++) {
            everytimeUser->setUserStatus(false);
        }

        for(onlineUser=remoteVector.begin(); onlineUser<remoteVector.end(); onlineUser++) {
            if(onlineUser->id==sessionData->userId)
                continue;

            bool found = false;
            for(everytimeUser=sessionData->usersList.begin(); everytimeUser<sessionData->usersList.end(); everytimeUser++) {
                if(*everytimeUser==*onlineUser) {
                    everytimeUser->setUserStatus(true);
                    found = true;
                    break;
                }
            }
            if(!found) {
                UserTag userTag;
                userTag.setUserId(std::stoi(onlineUser->id));
                userTag.setUserColor(LoginWindow::chooseColorFromString(QString::fromStdString(onlineUser->email)));
                userTag.setUsername(QString::fromStdString(onlineUser->username));
                userTag.setAvatar(LoginWindow::recoverImageFromEncodedString(QString::fromStdString(onlineUser->image)));
                userTag.setUserStatus(true);

                sessionData->usersList.push_back(userTag);
                sessionData->userColorMap[userTag.getUserId()] = userTag.getUserColor();
            }

            int pos = Crdt::getInstance().findAbsolutePosition(onlineUser->lastCursorPosition);
            QColor color = LoginWindow::chooseColorFromString(QString::fromStdString(onlineUser->email));
            ui->textEditShared->createRemoteCursor(std::stoi(onlineUser->id),pos,QString::fromStdString(onlineUser->username),color);
        }
        sessionData->onlineUsers = remoteVector;
        std::sort(sessionData->onlineUsers.begin(), sessionData->onlineUsers.end());

        populateUserTagList();
    } else {
        sessionData->onlineUsers = remoteVector;
    }

    for (auto user : remoteVector) {
        if (user.id != sessionData->userId) {
            int pos = Crdt::getInstance().findAbsolutePosition(user.lastCursorPosition);
            ui->textEditShared->updateRemoteCursorPosition(std::stoi(user.id), pos);
        }
    }
}

void MainWindow::exitFromEditor() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit from Simulpad",
                                                              "Are you sure you want to quit?");
    if (reply == QMessageBox::Yes) {
        this->close();
    }
}

void MainWindow::backToLogin() {
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Back to Login","Are you sure you want to logout?");

    if(reply == QMessageBox::Yes) {
        emit stopQueryLoop();
        Crdt::getInstance().reset();
        SessionData::accessToSessionData().loginWindowPointer->switchFrame(-1);
        SessionData::accessToSessionData().onlineUsers.clear();
        SessionData::accessToSessionData().usersList.clear();
        SessionData::accessToSessionData().userColorMap.clear();
        SessionData::accessToSessionData().youWannaLogin = true;
        SessionData::accessToSessionData().isLoginCorrect = false;

        this->close();
    }
}

void MainWindow::backToPersonalPage() {
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Back to your personal page","Are you sure you want to close this document?");

    if(reply == QMessageBox::Yes) {
        emit stopQueryLoop();
        Crdt::getInstance().reset();
        SessionData::accessToSessionData().onlineUsers.clear();
        SessionData::accessToSessionData().usersList.clear();
        SessionData::accessToSessionData().userColorMap.clear();
        SessionData::accessToSessionData().youWannaLogin = true;

        this->close();
    }
}

void MainWindow::exportPDF() {
    QString file_path = QFileDialog::getSaveFileName(this, "Export to PDF..", "", "Pdf file (*.pdf);;All Files (*)");

    if (file_path != "") {
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(file_path);
        printer.setPaperSize(QPrinter::A4);
        printer.setPageSizeMM(QSizeF(228.5,317));
        ui->textEditShared->document()->print(&printer);
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

    ui->textEditShared->document()->blockSignals(true);
    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
    ui->textEditShared->document()->blockSignals(false);
    Action a = Action(AlignLeft);
    Crdt::getInstance().sendActionToServer(a, cursor.selectionStart(), cursor.selectedText().length());
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

    ui->textEditShared->document()->blockSignals(true);
    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
    ui->textEditShared->document()->blockSignals(false);

    Action a = Action(AlignCenter);
    Crdt::getInstance().sendActionToServer(a, cursor.selectionStart(), cursor.selectedText().length());
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

    ui->textEditShared->document()->blockSignals(true);
    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
    ui->textEditShared->document()->blockSignals(false);

    Action a = Action(AlignRight);
    Crdt::getInstance().sendActionToServer(a, cursor.selectionStart(), cursor.selectedText().length());
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

    ui->textEditShared->document()->blockSignals(true);
    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);
    ui->textEditShared->document()->blockSignals(false);

    Action a = Action(AlignJustify);
    Crdt::getInstance().sendActionToServer(a, cursor.selectionStart(), cursor.selectedText().length());
}


void MainWindow::makeBold() {
    auto textEdit = ui->textEditShared;
    auto cursor = textEdit->textCursor();

    auto is_bold = ui->actionBold->isChecked();

    ui->actionBold->setChecked(is_bold);
    QTextCharFormat format;
    (is_bold) ? format.setFontWeight(QFont::Bold) : format.setFontWeight(QFont::Normal);
    cursor.mergeCharFormat(format);
    textEdit->setTextCursor(cursor);

    if (cursor.selectedText().length() > 0) {
        Action a = Action(0, is_bold ? 1 : 0);
        Crdt::getInstance().sendActionToServer(a, cursor.selectionStart() + 1, cursor.selectedText().length());
    }
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

    if (cursor.selectedText().length() > 0) {
        Action a = Action(1, is_italic ? 1 : 0);
        Crdt::getInstance().sendActionToServer(a, cursor.selectionStart() + 1, cursor.selectedText().length());
    }
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

    if (cursor.selectedText().length() > 0) {
        Action a = Action(2, is_underlined ? 1 : 0);
        Crdt::getInstance().sendActionToServer(a, cursor.selectionStart() + 1, cursor.selectedText().length());
    }
}

void MainWindow::textChanged(int pos, int nDel, int nIns) {

    if(SessionData::accessToSessionData().skipChanges)
        return;

    if(!ui->textEditShared->textCursor().hasSelection()) {
        if (lastEventType == QEvent::InputMethod)   //no idea what are these events and why they arrive here
            return;
        //IMPORTANT: Pasting text other than when the document is empty will result in unexpected behaviour
        //https://bugreports.qt.io/browse/QTBUG-3495?focusedCommentId=264621&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel#comment-264621

        if((pos == 0 || nIns > ui->textEditShared->document()->characterCount())  && nIns > 1 ) {   //workaround of a CTRL-V bug.
            auto clipboard = QApplication::clipboard();
            QString originalText = clipboard->text();
            std::string s = ui->textEditShared->textCursor().selectedText().toStdString();
            nIns = clipboard->text().count();
            //  ui->textEditShared->previousSelection.pop();
            nDel = ui->textEditShared->previousSelection;
        }

        if (nDel > 0) { //deletion
            Action action;
            Crdt::getInstance().sendActionToServer(action, pos + 1, nDel);
        }

        if (nIns > 0) { //insertion
            QString str = ui->textEditShared->document()->toPlainText().mid(pos, nIns);
            //check all properties of inserted chars
            auto text_cursor = ui->textEditShared->textCursor();
            if (nIns > 1)
                text_cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, nIns);
            auto format = text_cursor.charFormat();

            auto font = format.font();
            QString fontSize = QString::number(font.pointSize());
            int sizeIndex = ui->textEditShared->getFontSizes().indexOf(fontSize);
            int familyIndex = ui->textEditShared->getFontFamilies().indexOf(font.family());
            setComboSize(sizeIndex);

            bool bold = font.bold();
            bool italic = font.italic();
            bool underlined = font.underline();

            BlockFormatType blockFormatType;
            QTextBlockFormat blockFormat = text_cursor.blockFormat();
            Qt::Alignment alignment = blockFormat.alignment();
            if (alignment == Qt::AlignLeft)
                blockFormatType = AlignLeft;
            else if (alignment == Qt::AlignCenter)
                blockFormatType = AlignCenter;
            else if (alignment == Qt::AlignRight)
                blockFormatType = AlignRight;
            else if (alignment == Qt::AlignJustify)
                blockFormatType = AlignJustify;
            if (nIns > 1)
                text_cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, nIns);
            //following part is for pasting multiple blocks, creates 2 actions because the alignment of the cursor is kept only for the first pasted block
            if (blockFormatType != AlignLeft) {
                int i = 0;
                for (; i < str.size(); ++i) {
                    if (str[i] == '\n') {
                        ++i;
                        break;
                    }
                }
                if (i < str.size()) {
                    Action action(str.left(i), familyIndex, sizeIndex, bold, italic, underlined, blockFormatType);
                    Crdt::getInstance().sendActionToServer(action, pos, i);
                    pos += i;
                    nIns = str.size() - i;
                    str = str.right(str.size() - i);
                    blockFormatType = AlignLeft;
                }
            }

            Action action(str, familyIndex, sizeIndex, bold, italic, underlined, blockFormatType);
            Crdt::getInstance().sendActionToServer(action, pos, nIns);
        }
        for(auto user : SessionData::accessToSessionData().onlineUsers){
            if(user.id != SessionData::accessToSessionData().userId) {
                int pos = Crdt::getInstance().findAbsolutePosition(user.lastCursorPosition);
                ui->textEditShared->updateRemoteCursorPosition(std::stoi(user.id), pos);
            }
        }
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
    textEdit->setFocus();

    Action a = Action(3, familyIndex);
    if (cursor.selectedText().length())
        Crdt::getInstance().sendActionToServer(a, cursor.selectionStart() + 1, cursor.selectedText().length());
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
    textEdit->setFocus();

    Action a = Action(4, sizeIndex);
    if (cursor.selectedText().length())
        Crdt::getInstance().sendActionToServer(a, cursor.selectionStart() + 1, cursor.selectedText().length());
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

    ui->statusBar->findChild<QLabel*>("charCount")->setText("Chars: "+QString::number(ui->textEditShared->document()->characterCount()-1));
    ui->statusBar->findChild<QLabel*>("blockCount")->setText("Block: "+QString::number(ui->textEditShared->document()->blockCount()));
    ui->statusBar->findChild<QLabel*>("cursorPos")->setText("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    ui->statusBar->findChild<QLabel*>("cursorColumn")->setText("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    ui->statusBar->findChild<QLabel*>("cursorSelectionCount")->setText("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));

    SessionData::accessToSessionData().mutex_cursor_pos.lock();
    SessionData::accessToSessionData().cursor = Crdt::getInstance().findRelativePosition(text_cursor.position());
    SessionData::accessToSessionData().mutex_cursor_pos.unlock();

    ui->textEditShared->setTextColor({Qt::black});
    ui->textEditShared->setTextBackgroundColor({Qt::transparent});
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
                sendInvitationEmail(QString::fromStdString(SessionData::accessToSessionData().docName), dialog.textValue());
                break;
            }
        }
        else break;
    }
}

void MainWindow::changeEditorStatus()
{
    SessionData::accessToSessionData().mutex_online.lock();
    if(SessionData::accessToSessionData().isUserOnline && SessionData::accessToSessionData().offlineCounter<8) {
        SessionData::accessToSessionData().offlineCounter++;
        SessionData::accessToSessionData().mutex_online.unlock();
        return;
    }

    bool isUserOnline = !SessionData::accessToSessionData().isUserOnline;
    SessionData::accessToSessionData().isUserOnline = isUserOnline;

    //operations to do BEFORE the editor changes its status
    if(isUserOnline) {  //operations to do when back online
        ui->textEditShared->clearDocument();
    } else {    //operations to do when it turns offline
        if(ui->listOfflineUsers->isVisible())
            ui->offlineRollButton->click();
        if(ui->listOnlineUsers->isVisible())
            ui->onlineRollButton->click();
    }
    ui->textEditShared->setEnabled(isUserOnline);
    ui->statusBar->setEnabled(isUserOnline);
    ui->actionCopy->setEnabled(isUserOnline);
    ui->actionCut->setEnabled(isUserOnline);
    ui->actionPaste->setEnabled(isUserOnline);
    auto comboBoxes = ui->mainToolBar->findChildren<QComboBox*>();
    comboBoxes[0]->setEnabled(isUserOnline);
    comboBoxes[1]->setEnabled(isUserOnline);
    ui->actionBold->setEnabled(isUserOnline);
    ui->actionItalic->setEnabled(isUserOnline);
    ui->actionUnderlined->setEnabled(isUserOnline);
    ui->actionAlignLeft->setEnabled(isUserOnline);
    ui->actionAlignCenter->setEnabled(isUserOnline);
    ui->actionAlignRight->setEnabled(isUserOnline);
    ui->actionAlignJustify->setEnabled(isUserOnline);
    ui->actionColor->setEnabled(isUserOnline);
    ui->listOnlineUsers->setEnabled(isUserOnline);
    ui->listOfflineUsers->setEnabled(isUserOnline);
    ui->onlineRollButton->setEnabled(isUserOnline);
    ui->offlineRollButton->setEnabled(isUserOnline);

    //operations to do POST the editor changes its status
    if(isUserOnline) {  //operations to do when back online
        if(!ui->listOfflineUsers->isVisible())
            ui->offlineRollButton->click();
        if(!ui->listOnlineUsers->isVisible())
            ui->onlineRollButton->click();

        ui->myLed->setPixmap(QPixmap(QString::fromUtf8(":/resources/greenLed.png")));
        ui->myStatus->setText("Online");
    } else {    //operations to do when it turns offline
        ui->myLed->setPixmap(QPixmap(QString::fromUtf8(":/resources/redLed.png")));
        ui->myStatus->setText("Offline");

        Crdt::getInstance().reset();
    }
    SessionData::accessToSessionData().mutex_online.unlock();
}

void MainWindow::setupFontComboBoxes(QComboBox* comboSize, QComboBox* comboFamily)
{
    QFontDatabase::addApplicationFont(":/resources/fonts/Arial.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Arial Black.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Berlin Sans FB.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Calibri.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Century Gothic.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Consolas.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Constantia.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Forte.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Freestyle Script.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Georgia.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Gill Sans MT.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Harrington.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Informal Roman.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Lucida Calligraphy.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Palatino Linotype.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Segoe Script.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Tahoma.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Times New Roman.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Verdana.ttf");
    QFontDatabase::addApplicationFont(":/resources/fonts/Vivaldi.ttf");

    auto font = comboSize->font();
    font.setPointSize(13);
    comboSize->setFont(font);
    comboFamily->setFont(font);

    comboSize->addItems(ui->textEditShared->getFontSizes());
    comboSize->setCurrentText(QString::number(ui->textEditShared->currentFont().pointSize()));

    comboFamily->addItems(ui->textEditShared->getFontFamilies());
    QIcon icon(":/resources/font_icon.png");
    for(int i=0; i<comboFamily->count(); i++) {
        comboFamily->setItemIcon(i, icon);
    }
    comboFamily->setCurrentText(ui->textEditShared->currentFont().family());

    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[5], comboFamily);
    ui->mainToolBar->insertWidget(ui->mainToolBar->actions()[6], comboSize);
}

void MainWindow::setupStatusBar()
{
    auto docName = new QLabel("Document: "+ui->textEditShared->getDocumentName()+".sim");
    docName->setObjectName("docName");
    auto charCount = new QLabel("Chars: "+QString::number(ui->textEditShared->document()->characterCount()-1));
    charCount->setObjectName("charCount");
    auto blockCount = new QLabel("Blocks: "+QString::number(ui->textEditShared->document()->blockCount()));
    blockCount->setObjectName("blockCount");
    auto cursorPos = new QLabel("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    cursorPos->setObjectName("cursorPos");
    auto cursorColumn = new QLabel("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    cursorColumn->setObjectName("cursorColumn");
    auto cursorSelectionCount = new QLabel("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));
    cursorSelectionCount->setObjectName("cursorSelectionCount");

    ui->statusBar->addWidget(docName, 2);
    ui->statusBar->addWidget(charCount, 0);
    ui->statusBar->addWidget(blockCount, 1);
    ui->statusBar->addWidget(cursorPos, 0);
    ui->statusBar->addWidget(cursorColumn, 0);
    ui->statusBar->addWidget(cursorSelectionCount, 2);
}

void MainWindow::sendInvitationEmail(QString docName, QString destEmailAddress)
{
    QString base64Name= docName.toLocal8Bit().toBase64(QByteArray::OmitTrailingEquals);

    SmtpClient smtp;
    smtp.setUser("simulpad.texteditor@gmail.com");
    smtp.setPassword("progettomalnati");
    smtp.setMailSender("simulpad.texteditor@gmail.com","SimulPad");
    smtp.setMailDestination(destEmailAddress,"no_name");
    smtp.setMailSubject("Someone invites to collaborate on SimulPad");
    smtp.prepareMailText("ftp://simulpad.archive/document#"+base64Name);

    if(smtp.connectToHost()) {
        if(smtp.login()) {
            if(smtp.sendMail()) {
                QMessageBox::information(this,"Succeed","Your invitation was sent to "+destEmailAddress);
            }
        }
    }
    smtp.quit();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {

    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // disabling/ the ability to send enter in the document
        if (keyEvent->key() == Qt::Key_Shift) {
            if (keyEvent->type() == QEvent::KeyPress) {
                SessionData::accessToSessionData().go_down = false;
            } else {
                SessionData::accessToSessionData().go_down = true;
            }
        }
    }

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

        // based on enter send enter
        if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) {
            if (!SessionData::accessToSessionData().go_down)
                return true;
        }

        return false;
    } else {
        lastEventType = event->type();
        return false;
    }
}

void MainWindow::on_onlineRollButton_clicked()
{
    ui->listOnlineUsers->setVisible(!ui->listOnlineUsers->isVisible());
    if(ui->listOnlineUsers->isVisible()) {
        ui->onlineRollButton->setIcon(QIcon(":/resources/arrow_up.png"));
    }
    else {
        ui->onlineRollButton->setIcon(QIcon(":/resources/arrow_down.png"));
    }
}
void MainWindow::on_offlineRollButton_clicked()
{
    ui->listOfflineUsers->setVisible(!ui->listOfflineUsers->isVisible());
    if(ui->listOfflineUsers->isVisible()) {
        ui->offlineRollButton->setIcon(QIcon(":/resources/arrow_up.png"));
    }
    else {
        ui->offlineRollButton->setIcon(QIcon(":/resources/arrow_down.png"));
    }
}