/////////////////////////////////////////////////////////////////////////////
// Name:        mainwindow.cpp
// Purpose:     The main window
// Author:      Jan Buchholz
// Created:     2025-11-23
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileInfo>
#include "stylesheets.h"
#include "constants.h"
#include "jbpreferences.h"
#include "aboutdialog.h"
#include "io.h"
#include "conversions.hpp"
#include <QDir>
#include "fixedsplitter.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mc_dialogs = new Dialogs(this);
    ui->mainAppInfo->setToolTip(tr("About") + " " + APPNAME);
    ui->mainAppInfo->setText(tr("About") + " " + APPNAME);
    createToolBar();
    createMdViewer();
    createMdEditor();
    createSplitter();
    createStatusBar();
    populateMainMenu();
    setCentralWidget(m_mainSplitter);
    setUnifiedTitleAndToolBarOnMac(true);
    mc_synchronizer = new ScrollSynchronizer(m_mdEditor, m_mdViewer, m_mainSplitter);
    mc_synchelper = new SyncHelper(m_mdEditor, m_mdViewer);
    loadPreferences();
    setSync(m_sync);
    m_timer = new QTimer;
    m_timer->setSingleShot(true);
    m_timer->setInterval(250);
    setConnections(); 
    fileNew();
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_fontComboBox;
    delete m_fontSizeBox;
    delete m_mainToolBar;
    delete m_statusBar;
    delete m_mdViewer;
    delete m_mdEditor;
    delete m_timer;
    delete m_mainSplitter;
    delete mc_dialogs;
    delete mc_synchelper;
}

void MainWindow::createToolBar() {
    m_mainToolBar = new QToolBar;
    m_mainToolBar->setObjectName("mainToolbar");
    m_mainToolBar->setMovable(false);
    m_mainToolBar->setOrientation(Qt::Horizontal);
    m_mainToolBar->setAutoFillBackground(true);
    m_mainToolBar->setIconSize(QSize(DEF_ICONSIZE, DEF_ICONSIZE));
    m_mainToolBar->setFixedHeight(26);
#if defined(Q_OS_WIN)
    m_mainToolBar->setStyleSheet(styleToolBar);
#elif defined(Q_OS_MAC)
    m_mainToolBar->setStyleSheet(styleToolButton);
#endif
    this->addToolBar(Qt::TopToolBarArea, m_mainToolBar);
    m_mainToolBar->addAction(ui->mainNewFile);
    m_mainToolBar->addAction(ui->mainOpenFile);
    m_mainToolBar->addAction(ui->mainSaveFile);
    m_mainToolBar->addAction(ui->mainSaveFileAs);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(ui->mainEditCopy);
    m_mainToolBar->addAction(ui->mainEditCut);
    m_mainToolBar->addAction(ui->mainEditPaste);
    m_mainToolBar->addAction(ui->mainEditSelectAll);
    m_mainToolBar->addAction(ui->mainEditUndo);
    m_mainToolBar->addAction(ui->mainEditRedo);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(ui->mainEditFormatBold);
    m_mainToolBar->addAction(ui->mainEditFormatItalic);
    m_mainToolBar->addAction(ui->mainEditFormatStrikethrough);
    m_mainToolBar->addAction(ui->mainEditFormatCode);
    m_mainToolBar->addAction(ui->mainEditFormatHighlight);
    QWidget* spacerSmall = new QWidget;
    spacerSmall->setMinimumWidth(25);
    m_mainToolBar->addWidget(spacerSmall);
    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    m_fontComboBox->setWritingSystem(QFontDatabase::Latin);
    m_mainToolBar->addWidget(m_fontComboBox);
    m_fontSizeBox = new QComboBox;
    m_fontSizeBox->addItems(fontSizeList);
    m_mainToolBar->addWidget(m_fontSizeBox);
    QWidget* spacerLarge = new QWidget;
    spacerLarge->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mainToolBar->addWidget(spacerLarge);
    m_mainToolBar->addAction(ui->mainAppInfo);
    m_mainToolBar->addAction(ui->mainAppQuit);
}

void MainWindow::createMdViewer() {
    m_mdViewer = new QTextBrowser;
    m_mdViewer->setWordWrapMode(QTextOption::WordWrap);
    m_mdViewer->setLineWrapMode(QTextEdit::WidgetWidth);
    m_mdViewer->setAcceptRichText(false);
    m_mdViewer->setOpenExternalLinks(true);
}

void MainWindow::createMdEditor() {
    m_mdEditor = new QTextEdit;
    m_mdEditor->setAcceptRichText(false);
    m_mdEditor->setWordWrapMode(QTextOption::WordWrap);
    m_mdEditor->setLineWrapMode(QTextEdit::WidgetWidth);
    m_mdEditor->setOverwriteMode(false);
    m_mdEditor->setUndoRedoEnabled(true);
}

void MainWindow::createSplitter() {
    m_mainSplitter = new FixedSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(m_mdEditor);
    m_mainSplitter->addWidget(m_mdViewer);
    m_mainSplitter->setSizes({1, 1});
}

void MainWindow::createStatusBar() {
    m_statusBar = new QStatusBar;
    this->setStatusBar(m_statusBar);
#if defined(Q_OS_MAC)
    m_statusBar->setVisible(false);
#endif
}

void MainWindow::populateMainMenu() {
    ui->menuFile->addAction(ui->mainNewFile);
    ui->menuFile->addAction(ui->mainOpenFile);
    ui->menuFile->addAction(ui->mainSaveFile);
    ui->menuFile->addAction(ui->mainSaveFileAs);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(ui->mainAppQuit);
    ui->menuEdit->addAction(ui->mainEditCopy);
    ui->menuEdit->addAction(ui->mainEditCut);
    ui->menuEdit->addAction(ui->mainEditPaste);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditSelectAll);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditUndo);
    ui->menuEdit->addAction(ui->mainEditRedo);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->mainEditFormatBold);
    ui->menuEdit->addAction(ui->mainEditFormatItalic);
    ui->menuEdit->addAction(ui->mainEditFormatStrikethrough);
    ui->menuEdit->addAction(ui->mainEditFormatCode);
    ui->menuEdit->addAction(ui->mainEditFormatHighlight);
    ui->menuOptions->addAction(ui->mainOptionsSync);
    ui->menuHelp->addAction(ui->mainAppInfo);
}

void MainWindow::setConnections() {
    connect(ui->mainNewFile, &QAction::triggered, this, &MainWindow::onNewFile);
    connect(ui->mainOpenFile, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->mainSaveFile, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->mainSaveFileAs, &QAction::triggered, this, &MainWindow::onSaveFileAs);
    connect(ui->mainEditCopy, &QAction::triggered, this, &MainWindow::onEditCopy);
    connect(ui->mainEditCut, &QAction::triggered, this, &MainWindow::onEditCut);
    connect(ui->mainEditPaste, &QAction::triggered, this, &MainWindow::onEditPaste);
    connect(ui->mainEditSelectAll, &QAction::triggered, this, &MainWindow::onEditSelectAll);
    connect(ui->mainEditUndo, &QAction::triggered, this, &MainWindow::onEditUndo);
    connect(ui->mainEditRedo, &QAction::triggered, this, &MainWindow::onEditRedo);
    connect(ui->mainEditFormatBold, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatItalic, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatStrikethrough, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatCode, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainEditFormatHighlight, &QAction::triggered, this, &MainWindow::onEditFormat);
    connect(ui->mainOptionsSync, &QAction::toggled, this, &MainWindow::onOptionsSync);
    connect(ui->mainAppInfo, &QAction::triggered, this, &MainWindow::onAppInfo);
    connect(ui->mainAppQuit, &QAction::triggered, this, &MainWindow::onAppQuit);
    connect(m_fontComboBox, &QFontComboBox::currentFontChanged, this, &MainWindow::onCurrentFontChanged);
    connect(m_fontSizeBox, &QComboBox::currentTextChanged, this, &MainWindow::onCurrentTextChanged);
    connect(m_mdEditor, &QTextEdit::textChanged, this, [this]() { m_timer->start(); });
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTextChanged);
}

void MainWindow::savePreferences() {
    JBPreferences *prefs = new JBPreferences();
    prefs->PushArray(SET_WGEOMETRY, saveGeometry());
    prefs->PushArray(SET_WSTATE, saveState(0));
    prefs->PushBoolean(SET_SYNC, m_sync);
    prefs->PushString(SET_LASTFOLDER, m_lastFolder);
    prefs->PushFont(SET_EDITORFONT, m_mdViewer->font());
    prefs->SavePreferencesToDefaultLocation(SET_COMPANY, APPNAME);
    delete prefs;
}

void MainWindow::loadPreferences() {
    JBPreferences *prefs = new JBPreferences();
    if (prefs->LoadPreferencesFromDefaultLocation(SET_COMPANY, APPNAME)) {
        try {
            restoreGeometry(prefs->PopArray(SET_WGEOMETRY));
            restoreState(prefs->PopArray(SET_WSTATE));
            m_sync = prefs->PopBoolean(SET_SYNC);
            m_lastFolder = prefs->PopString(SET_LASTFOLDER);
            QFont efont = prefs->PopFont(SET_EDITORFONT);
            if (!efont.family().isEmpty()) {
                m_fontComboBox->setCurrentFont(efont);
                QString esize = QString::number(efont.pointSize());
                if (fontSizeList.indexOf(esize) >= 0) m_fontSizeBox->setCurrentText(esize);
                m_mdViewer->setFont(efont);
                m_mdEditor->setFont(efont);
            }
        }
        catch (...) {}
    }
    delete prefs;
    if (m_lastFolder.isEmpty()) m_lastFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if (answer == QMessageBox::Cancel) {
            e->ignore();
            return;
        }
        if (answer == QMessageBox::Yes) {
            if (!fileSave()) {
                e->ignore();
                return;
            }
        }
    }
    savePreferences();
    setWindowModified(false);
    e->accept();
}

void MainWindow::onTextChanged() {
    if (m_dirty) setWindowModified(true);
    mc_synchelper->syncToViewer();
    if (!m_dirty) m_dirty = true;
}

// font combobox
void MainWindow::onCurrentFontChanged(const QFont font) {
    QFont f = font;
    if (!f.family().isEmpty()) {
        int s = m_fontSizeBox->currentText().toInt();
        f.setPointSize(s);
        m_mdViewer->setFont(f);
        m_mdEditor->setFont(f);
    }
}

// font size combobox
void MainWindow::onCurrentTextChanged(const QString size) {
    int s = size.toInt();
    QFont font = m_mdViewer->font();
    font.setPointSize(s);
    m_mdViewer->setFont(font);
    m_mdEditor->setFont(font);
}

// enable/disable scroll sync.
void MainWindow::onOptionsSync(bool checked) {
    setSync(checked);
}

void MainWindow::onNewFile() {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if(answer == QMessageBox::Cancel) return;
        if(answer == QMessageBox::Yes) {
            if (!fileSave()) return;
        }
    }
    fileNew();
}

void MainWindow::onOpenFile() {
    fileOpen();
}

void MainWindow::onSaveFile() {
    fileSave();
}

void MainWindow::onSaveFileAs() {
    fileSave(true);
}

void MainWindow::onEditCopy() {
    m_mdEditor->copy();
}

void MainWindow::onEditCut() {
    m_mdEditor->cut();
}

void MainWindow::onEditPaste() {
    if (m_mdEditor->canPaste()) m_mdEditor->paste();
}

void MainWindow::onEditSelectAll() {
    m_mdEditor->selectAll();
}

void MainWindow::onEditUndo() {
    m_mdEditor->undo();
}

void MainWindow::onEditRedo() {
    m_mdEditor->redo();
}

void MainWindow::onEditFormat() {
    QTextCursor cursor = m_mdEditor->textCursor();
    if (cursor.hasSelection()) {
        int s_begin = cursor.selectionStart();
        int s_end = cursor.selectionEnd();
        QAction* action = qobject_cast<QAction*>(sender());
        if (!action) return;
        QString format{};
        if (action == ui->mainEditFormatBold) {
            format = "**";
        } else if (action == ui->mainEditFormatItalic) {
            format = "*";
        } else if (action == ui->mainEditFormatStrikethrough) {
            format = "~";
        } else if (action == ui->mainEditFormatCode) {
            format = "`";
        } else if (action == ui->mainEditFormatHighlight) {
            format = "==";
        }
        if (!format.isEmpty()) {
            cursor.setPosition(s_end, QTextCursor::MoveAnchor);
            cursor.insertText(format);
            cursor.setPosition(s_begin, QTextCursor::MoveAnchor);
            cursor.insertText(format);
        }
    }
}

void MainWindow::onAppInfo() {
    AboutDialog dlg;
    dlg.setParent(this);
    int pw = width();
    int ph = height() + m_mainToolBar->height();
    int mw = dlg.width();
    int mh = dlg.height();
    if (pw >= mw && ph >= mh) {
        dlg.move((pw - mw) / 2, (ph - mh) / 2);
    }
    dlg.exec();
}

void MainWindow::onAppQuit() {
    close();
}

void MainWindow::fileNew() {
    m_fileName.clear();
    m_mdEditor->blockSignals(true);
    m_mdEditor->clear();
    m_mdViewer->clear();
    m_mdEditor->blockSignals(false);
    mc_synchelper->invalidateImageCache();
    QString title = QString(APPNAME) + " - " + mc_dialogs->UNTITLED_DOCUMENT + PLACEHOLDER;
    setWindowTitle(title);
    setWindowModified(false);
    m_dirty = true;
    m_mdEditor->setFocus();
}

bool MainWindow::fileOpen() {
    if (isWindowModified()) {
        int answer = mc_dialogs->showDialogUnsavedChanges();
        if (answer == QMessageBox::Cancel) return true;
        if (answer == QMessageBox::Ok) {
            return fileSave();
        }
    }
    QString fName = mc_dialogs->showOpenFileDialog(m_lastFolder);
    if (!fName.isEmpty()) {
        QFileInfo fileInfo(fName);
        m_lastFolder = fileInfo.path();
        m_fileName = fileInfo.fileName();
        QByteArray ba;
        Io* io = new Io();
        QString result = io->load(fName, &ba);
        delete io;
        if (result.isEmpty()) {
            mc_synchelper->invalidateImageCache();
            mc_synchelper->setDocumentPath(m_lastFolder);
            m_dirty = false;
            m_mdEditor->setPlainText(ByteArrayToString(ba));
            QString title = QString(APPNAME) + " - " + fileInfo.fileName() + PLACEHOLDER;
            setWindowTitle(title);
            setWindowModified(false);
            return true;
        } else mc_dialogs->showDialogDisplayError(result);
    }
    return false;
}

bool MainWindow::fileSave(bool saveAs) { //saveAs default false
    QString fName;
    if (m_lastFolder.isEmpty() || m_fileName.isEmpty() || saveAs) {
        fName = mc_dialogs->showSaveFileDialog(m_lastFolder, m_fileName);
        if (fName.isEmpty()) return false;
        QFileInfo fileInfo(fName);
        m_lastFolder = fileInfo.path();
        mc_synchelper->setDocumentPath(m_lastFolder);
        m_fileName = fileInfo.fileName();
    } else {
        QFileInfo fileInfo(m_lastFolder, m_fileName);
        fName = fileInfo.absoluteFilePath();
    }
    QByteArray ba = StringToByteArray(m_mdEditor->toPlainText());
    Io* io = new Io();
    QString result = io->save(fName, ba);
    delete io;
    if (result.isEmpty()) {
        mc_synchelper->refreshDocument();
        QString title = QString(APPNAME) + " - " + m_fileName + PLACEHOLDER;
        setWindowTitle(title);
        setWindowModified(false);
        return true;
    } else mc_dialogs->showDialogDisplayError(result);
    return false;
}

void MainWindow::setSync(bool enabled) {
    m_sync = enabled;
    mc_synchronizer->setEnabled(m_sync);
    if (enabled) {
        ui->mainOptionsSync->setText(tr("Disable scrolling synchronization"));
    } else {
        ui->mainOptionsSync->setText(tr("Enable scrolling synchronization"));
    }
}
