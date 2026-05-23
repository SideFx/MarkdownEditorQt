/////////////////////////////////////////////////////////////////////////////
// Name:        scrollsynchronizer.h
// Purpose:     Synchronize scrolling of MD editor with MD viewer (header)
// Author:      Jan Buchholz
// Created:     2025-11-20
// Changed:     2026-05-23
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QSplitter>
#include <QTextEdit>
#include <QMainWindow>
#include <QTextBrowser>

class ScrollSynchronizer : public QObject {
    Q_OBJECT

public:
    explicit ScrollSynchronizer(QTextEdit* editor,
                                QTextBrowser* browser,
                                QMainWindow* parent);
    void setEnabled(bool enabled);

private:
    QTextEdit* m_editor;
    QTextBrowser* m_viewer;
    bool m_syncing;
    bool m_enabled;

private slots:
    void sync();
};


