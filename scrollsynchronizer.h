/////////////////////////////////////////////////////////////////////////////
// Name:        scrollsynchronizer.h
// Purpose:     Synchronize scrolling of MD editor with MD viewer (header)
// Author:      Jan Buchholz
// Created:     2025-11-20
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QSplitter>
#include <QTextEdit>
#include <QTextBrowser>

class ScrollSynchronizer : public QObject {
    Q_OBJECT

public:
    explicit ScrollSynchronizer(QTextEdit* editor, QTextBrowser* browser, QSplitter* parentSplitter);
    void setEnabled(bool enabled);

private:
    QTextEdit* m_editor;
    QTextBrowser* m_viewer;
    QSplitter* m_splitter;
    bool m_syncing;
    bool m_enabled;

private slots:
    void sync();
};


