/////////////////////////////////////////////////////////////////////////////
// Name:        synchelper.h
// Purpose:     Keep MD viewer in sync with MD editor (header)
// Author:      Jan Buchholz
// Created:     2025-11-26
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <QTextEdit>
#include <QTextBrowser>
#include "imagehandler.h"

class SyncHelper : public QObject {
    Q_OBJECT

public:
    SyncHelper(QTextEdit* editor, QTextBrowser* viewer);
    ~SyncHelper();
    void syncToViewer();
    void setDocumentPath(QString path) { m_documentPath = path; }
    void invalidateImageCache() { mc_handler->invalidateCache(); }

private:
    QTextEdit* m_editor;
    QTextBrowser* m_viewer;
    ImageHandler* mc_handler;
    QString m_documentPath;
    void processImages();

    const QString c_marker_editor = QString::fromUtf8("\u200B@@SYNC@@\u200B");
    const QString c_marker_viewer = "<a id=\"sync\" hidden>&nbsp;</a>";
    const QString c_anchor = "sync";
    const QString c_highlighting = "<span style=\"background-color:#FAE098\">\\1</span>";
};

