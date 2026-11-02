/////////////////////////////////////////////////////////////////////////////
// Name:        markdownapplication.h
// Purpose:     Handle macOS event and Windows singleton instance (header)
// Author:      Jan Buchholz
// Created:     2026-11-01
// Changed:     2026-11-02
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QApplication>
#include <QLocalServer>

class MainWindow;

class MarkdownApplication : public QApplication {
    Q_OBJECT
private:
    MainWindow* m_mainWindow = nullptr;
    QString m_pendingFilePath;
    QLocalServer* m_localServer = nullptr;
    const QString m_serverName = "org.janbuchholz.markdowneditorqt.lock";

public:
    MarkdownApplication(int &argc, char **argv) : QApplication(argc, argv) {}
    void setMainWindow(MainWindow *window);
    bool sendFileToRunningInstance(const QString& fileName);
    void startLocalServer(MainWindow *window);

protected:
    bool event(QEvent *event) override;

};

