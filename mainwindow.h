/////////////////////////////////////////////////////////////////////////////
// Name:        mainwindow.h
// Purpose:     The main window (header)
// Author:      Jan Buchholz
// Created:     2025-11-23
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QMainWindow>
#include <QToolBar>
#include <QSplitter>
#include <QCloseEvent>
#include <QFontComboBox>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTimer>
#include "dialogs.h"
#include "scrollsynchronizer.h"
#include "synchelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    QToolBar *m_mainToolBar;
    QStatusBar *m_statusBar;
    QTextBrowser *m_mdViewer;
    QTextEdit *m_mdEditor;
    QSplitter *m_mainSplitter;
    QFontComboBox *m_fontComboBox;
    QComboBox *m_fontSizeBox;
    ScrollSynchronizer *mc_synchronizer;
    QString m_lastFolder;
    QString m_fileName;
    QTimer *m_timer;
    Dialogs *mc_dialogs;
    SyncHelper *mc_synchelper;
    bool m_sync;
    bool m_dirty;
    const QStringList fontSizeList = {"8", "9", "10", "11", "12", "13", "14", "16", "18"};
    void createMdViewer();
    void createMdEditor();
    void createToolBar();
    void createSplitter();
    void createStatusBar();
    void populateMainMenu();
    void setConnections();
    void savePreferences();
    void loadPreferences();
    void fileNew();
    bool fileOpen();
    bool fileSave(bool saveAs = false);
    void setSync(bool enabled);

private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveFileAs();
    void onAppInfo();
    void onAppQuit();
    void onEditCopy();
    void onEditCut();
    void onEditPaste();
    void onEditSelectAll();
    void onEditUndo();
    void onEditRedo();
    void onEditFormat();

    void onOptionsSync(bool);
    void onCurrentFontChanged(const QFont);
    void onCurrentTextChanged(const QString);
    void onTextChanged();

};

