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
#include <QClipboard>
#include <QPrinter>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QPushButton>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    background_task(200)
{
    getSessionDataFromLogin();

    Crdt::getInstance().init(std::stoi(this->sessionData.userId));

    query = new OnlineQuery{this->sessionData.docId,this->sessionData.token,this};
    query->start();

    ui->setupUi(this);
    ui->myUsername->setText(QString::fromStdString(sessionData.username));
    ui->myAvatar->setPixmap(sessionData.avatar);
    ui->textEditShared->setAcceptRichText(false); //this needs to be false to avoid pasting formatted text with CTRL+V
    ui->textEditShared->installEventFilter(this);
    ui->textEditShared->setDocumentName(QString::fromStdString(sessionData.docName));
    Shared_editor::getInstance().initString(ui->textEditShared->toHtml());

    ui->sideLayout->layout()->setAlignment(Qt::AlignTop);

    auto comboSize = new QComboBox(ui->mainToolBar);
    auto comboFamily = new QComboBox(ui->mainToolBar);
    setupFontComboBoxes(comboSize, comboFamily);
    setupStatusBar();

    populateUserTagList();

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
    connect(ui->actionTestDisconnect,&QAction::triggered,this,&MainWindow::disableEditor); //only for test
    connect(ui->actionTestColor,&QAction::toggled,ui->textEditShared,&MyTextEdit::colorText);
}

MainWindow::~MainWindow()
{
    background_task.cancel();
    background_task.wait();
    delete ui;
}

void MainWindow::getSessionDataFromLogin() {
    this->sessionData = LoginWindow::getInstance().sessionData;
}

void MainWindow::populateUserTagList()
{
    QString statusLabel;
    QPixmap led;
    for(auto &tag : this->sessionData.usersList) {
        auto item = new QListWidgetItem();
        if(tag.getStatus()) {
            led.load(":/resources/greenLed.png");
            statusLabel = "Online";
            ui->listOnlineUsers->setItemDelegate(&tag);
            ui->listOnlineUsers->addItem(item);
        }
        else {
            led.load(":/resources/redLed.png");
            statusLabel = "Offline";
            ui->listOfflineUsers->setItemDelegate(&tag);
            ui->listOfflineUsers->addItem(item);
        }

        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(0,255);
        auto random_value = std::bind(distribution, generator);
        QColor color(random_value(),random_value(),random_value());

        item->setData(Qt::UserRole + 1, tag.getUsername());
        item->setData(Qt::UserRole + 2, statusLabel);
        item->setData(Qt::UserRole + 3, tag.getAvatar());
        item->setData(Qt::UserRole + 4, led);
        item->setData(Qt::UserRole + 5, tag.getUserColor());
    }
}

void MainWindow::update_id(std::string id){
    if (id.compare("")!=0)
        this->sessionData.lastCrdtId = id;
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

    Action a = Action( AlignLeft);
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

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);

    Action a = Action( AlignCenter);
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

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);

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

    cursor.mergeBlockFormat(textBlockFormat);
    textEdit->setTextCursor(cursor);

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

//TODO: create multiple actions when copy pasting text with different styles
void MainWindow::textChanged(int pos, int nDel, int nIns) {

    if(nDel==0) {  //insertion
        QString str = ui->textEditShared->document()->toPlainText().mid(pos, nIns);
//        for(int i=0; i<nIns; i++) {
//            str += ui->textEditShared->document()->characterAt(pos+i);
//        }
//        qDebug() << str;

        //check all properties of inserted chars
        auto text_cursor = ui->textEditShared->textCursor();
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
        Qt::Alignment alignment = text_cursor.blockFormat().alignment();
        if (alignment == Qt::AlignLeft)
            blockFormatType = AlignLeft;
        else if (alignment == Qt::AlignCenter)
                blockFormatType = AlignCenter;
        else if (alignment == Qt::AlignRight)
             blockFormatType = AlignRight;
        else if (alignment == Qt::AlignJustify)
             blockFormatType = AlignJustify;

        Action action(str, familyIndex, sizeIndex, bold, italic, underlined, blockFormatType);
        Crdt::getInstance().sendActionToServer(action, pos, nIns);

    } else if (nIns==0) { //deletion
        Action action;
        Crdt::getInstance().sendActionToServer(action, pos + 1, nDel);
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

    Action a = Action(3, familyIndex);
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

    Action a = Action(4, sizeIndex);
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
    ui->statusBar->findChild<QLabel*>("lineCount")->setText("Lines: "+QString::number(ui->textEditShared->document()->lineCount()));
    ui->statusBar->findChild<QLabel*>("cursorPos")->setText("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    ui->statusBar->findChild<QLabel*>("cursorColumn")->setText("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    ui->statusBar->findChild<QLabel*>("cursorSelectionCount")->setText("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));

    ui->textEditShared->setTextColor(QColor("black"));
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

void MainWindow::disableEditor()
{
    ui->textEditShared->setEnabled(!ui->textEditShared->isEnabled());
    ui->statusBar->setEnabled(!ui->statusBar->isEnabled());
    ui->actionCopy->setEnabled(!ui->actionCopy->isEnabled());
    ui->actionCut->setEnabled(!ui->actionCut->isEnabled());
    ui->actionPaste->setEnabled(!ui->actionPaste->isEnabled());
    auto comboBoxes = ui->mainToolBar->findChildren<QComboBox*>();
    comboBoxes[0]->setEnabled(!comboBoxes[0]->isEnabled());
    comboBoxes[1]->setEnabled(!comboBoxes[1]->isEnabled());
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
    auto lineCount = new QLabel("Lines: "+QString::number(ui->textEditShared->document()->lineCount()));
    lineCount->setObjectName("lineCount");
    auto cursorPos = new QLabel("pos: "+QString::number(ui->textEditShared->textCursor().position()));
    cursorPos->setObjectName("cursorPos");
    auto cursorColumn = new QLabel("col: "+QString::number(ui->textEditShared->textCursor().columnNumber()));
    cursorColumn->setObjectName("cursorColumn");
    auto cursorSelectionCount = new QLabel("sel: "+QString::number(ui->textEditShared->textCursor().selectedText().length()));
    cursorSelectionCount->setObjectName("cursorSelectionCount");

    ui->statusBar->addWidget(docName, 2);
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


void MainWindow::update_online_users_and_cursors_positions(std::vector<exchangeable_data::user> vector){
    // NB: è ancora da debuggare andando a creare dei dati fittizzi dentro userBar in modo
    // da verificare che tutto funzioni, il problema è che non so come creare degli oggetti di tipo
    // UserTag, perdonami ma senza di te mi è difficile utilizzarlo
    std::vector<UserTag>& usersBar = this->sessionData.usersList;
    std::vector<UserTag> userBarUpdated;

    for(exchangeable_data::user u : vector){
        auto iterator_user = std::find(usersBar.begin(),usersBar.end(),u);

        if (iterator_user != usersBar.end()){
            // the user was already present, i only copy it in the new bar updated
            UserTag user = *iterator_user; //  called copy constructor
            // TODO: dario check if is correct use true
            user.setUserStatus(true); // was more clear use like: user.setUserIsOnline(...) status is not auto explicative
            userBarUpdated.push_back(std::move(user)); // forwarded using movement semantic, std::move is optional, the compiler
            // is smart enought to understand it automatically, is only for you dario with love
        } else {
            // TODO: dario please call che constructor correctly i'm not sure of how to calc the data like color for eg
            // all the data required are inside object u
            std::cout << "add the user" << u.username << std::endl;
        }

    }
    // at this point userBarUpdated contains all the online users, to detect the offline i can subtract the online
    // from the original one

    // TODO: dario check also this comments please, if you do not undestand ask me! cit. elia
    // please for working the original data must be sorted in the same way that i will now use to sort the new list
    // this also is required for the draw operation otherwise the list will change from an instant to the next also
    // in the order
    std::sort(userBarUpdated.begin(),userBarUpdated.end(),[](const UserTag& u1,const UserTag& u2) -> bool {
        return u1.getUsername().compare(u2.getUsername()); // get... is java like syntax, cpp programmer tend to prefer more concise syntax in general
        // is absolutely not wrong, i had written only because the cpp general way of programming is that
    });

    std::vector<UserTag> difference;

    std::set_difference(
            usersBar.begin(),usersBar.end(),
            userBarUpdated.begin(),userBarUpdated.end(),
            std::back_inserter(difference)
            );

    for(UserTag& user : difference){
        user.setUserStatus(false); // TODO: dario check if correct, that are the users that are offline
        userBarUpdated.push_back(user);
    }

    // now i replace the old data with the new calculated
    this->sessionData.usersList = userBarUpdated;

    // TODO: now that data are updated need to be refreshed the interface

    for(exchangeable_data::user u : vector){
        std::cout << "manage the remote cursor json : " << u.lastCursorPositionJson << std::endl;
    }

    std::cout << "arrived new users and cursors, update the data!" << std::endl;
}

//TODO: clicking on bold/cursive/etc. with nothing highlighted sends an action but it shouldn't
//TODO: block formatting doesn't result in sending an action but it should