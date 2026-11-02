/////////////////////////////////////////////////////////////////////////////
// Name:        markdownapplication.h
// Purpose:     Handle macOS event and Windows singleton instance
// Author:      Jan Buchholz
// Created:     2026-11-01
// Changed:     2026-11-02
/////////////////////////////////////////////////////////////////////////////

#include "markdownapplication.h"
#include "mainwindow.h"
#include <QFileOpenEvent>
#include <QLocalSocket>

void MarkdownApplication::setMainWindow(MainWindow *window) {
    m_mainWindow = window;
    if (!m_pendingFilePath.isEmpty() && m_mainWindow) {
        m_mainWindow->openFile(m_pendingFilePath);
        m_pendingFilePath.clear();
    }
}

// macOS only, load file via event
bool MarkdownApplication::event(QEvent *event) {
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *openEvent = static_cast<QFileOpenEvent*>(event);
        QString filePath = openEvent->file();
        if (m_mainWindow) {
            m_mainWindow->openFile(filePath);
            //m_mainWindow->raise();
            //m_mainWindow->activateWindow();
        } else {
            m_pendingFilePath = filePath;
        }
        return true;
    }
    return QApplication::event(event);
}

bool MarkdownApplication::sendFileToRunningInstance(const QString& fileName) {
    QLocalSocket socket;
    socket.connectToServer(m_serverName);
    if (!socket.waitForConnected(500)) return false;
    socket.write(fileName.toUtf8());  //second instance -> pipe
    socket.flush();
    socket.waitForBytesWritten();
    return true;
}

void MarkdownApplication::startLocalServer(MainWindow *window) {
    m_localServer = new QLocalServer(window);
    QObject::connect(m_localServer, &QLocalServer::newConnection,
        [this, window]() {
            auto socket = m_localServer->nextPendingConnection();
            QObject::connect(socket, &QLocalSocket::readyRead, // read from pipe
                [window, socket]() {
                    QObject::connect(socket, &QLocalSocket::disconnected, // connection closed
                        [window, socket]() {
                        QString file = QString::fromUtf8(socket->readAll());
                        window->openFile(file); // filename received from second instance
                        //window->raise();
                        //window->activateWindow();
                        socket->deleteLater();
                    });
            });
    });
    QLocalServer::removeServer(m_serverName);
    m_localServer->listen(m_serverName);
}
