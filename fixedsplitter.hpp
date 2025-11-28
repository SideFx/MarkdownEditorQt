/////////////////////////////////////////////////////////////////////////////
// Name:        fixedsplitter.hpp
// Purpose:     QSplitter/QSplitterHandle subclasses
// Author:      Jan Buchholz
// Created:     2025-10-13
/////////////////////////////////////////////////////////////////////////////

#include <QSplitter>
#include <QSplitterHandle>
#include <QMouseEvent>
#include <QWidget>

class FixedSplitterHandle : public QSplitterHandle {
public:
    FixedSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
        : QSplitterHandle(orientation, parent) {}

protected:
    void enterEvent(QEnterEvent *event) override {
        unsetCursor();
        QSplitterHandle::enterEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        event->ignore();
    }
};

class FixedSplitter : public QSplitter {
public:
    FixedSplitter(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSplitter(orientation, parent) {}

protected:
    QSplitterHandle *createHandle() override {
        return new FixedSplitterHandle(orientation(), this);
    }
};

