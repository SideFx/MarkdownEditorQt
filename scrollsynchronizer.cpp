/////////////////////////////////////////////////////////////////////////////
// Name:        scrollsynchronizer.cpp
// Purpose:     Synchronize scrolling of MD editor with MD viewer
// Author:      Jan Buchholz
// Created:     2025-11-20
/////////////////////////////////////////////////////////////////////////////

#include "scrollsynchronizer.h"
#include <QScrollBar>

ScrollSynchronizer::ScrollSynchronizer(QTextEdit* editor, QTextBrowser* browser, QSplitter* parentSplitter)
    : QObject(parentSplitter), m_editor(editor), m_viewer(browser), m_splitter(parentSplitter), m_syncing(false) {
    connect(m_editor->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &ScrollSynchronizer::sync);
}

void ScrollSynchronizer::setEnabled(bool enabled) {
    m_enabled = enabled;
}

void ScrollSynchronizer::sync() {
    if (m_syncing || !m_enabled) return;
    m_syncing = true;
    QScrollBar* sourceBar = m_editor->verticalScrollBar();
    QScrollBar* targetBar = m_viewer->verticalScrollBar();
    int sourceRange = sourceBar->maximum() - sourceBar->minimum();
    int targetRange = targetBar->maximum() - targetBar->minimum();
    if (sourceRange > 0 && targetRange > 0) {
        double ratio = double(sourceBar->value() - sourceBar->minimum()) / sourceRange;
        int newValue = targetBar->minimum() + ratio * targetRange;
        if (std::abs(targetBar->value() - newValue) > 1) targetBar->setValue(newValue);
    }
    m_syncing = false;
}
