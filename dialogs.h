/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.h
// Purpose:     File- and message dialogs header
// Author:      Jan Buchholz
// Created:     2025-11-23
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>

class Dialogs : public QObject {
    Q_OBJECT

public:
    explicit Dialogs(QWidget *parent = nullptr);
    ~Dialogs();
    QString showOpenFileDialog(QString folder);
    QString showSaveFileDialog(QString folder, QString fileName);
    int showDialogUnsavedChanges();
    void showDialogDisplayError(QString message);

    const QString UNTITLED_DOCUMENT = tr("NoName.md");

private:
    QMainWindow *mainWindow;
    int yesNoCancelDialog(QString title, QString message, QMessageBox::Icon icon);
    QString const MSG_WANTSAVE = QObject::tr("Should the changes be saved before closing?\n"
                                             "If you don't save, the changes will be lost.");
    QString const FILE_FILTER = QObject::tr("Markdown files (*.md)");

};

