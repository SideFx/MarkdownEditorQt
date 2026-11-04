/////////////////////////////////////////////////////////////////////////////
// Name:        stylesheets.h
// Purpose:     Style sheets for QToolBar & QToolButton
// Author:      Jan Buchholz
// Created:     2025-10-13
// Changed:     2026-11-04
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QString>

QString styleToolBarMac = R"(
        QToolBar > QToolButton {
            border-radius: 3px;
            padding: 2px 2px;
        }
        QToolBar > QToolButton:hover {
            background-color: gray;
            border-color: auto;
        }
        QToolBar > QToolButton:pressed {
            background-color: darkGray;
            border-color: auto;
        }
        QToolBar > QToolButton:checked {
            background-color: darkGray;
            border-color: auto;
        }
        QToolBar {
            border: 0px;
            background-color: whiteSmoke;

        }
)";
QString styleStatusBarMac = R"(
        QStatusBar {
            border: 0px;
            background-color: whiteSmoke;
        }
)";
QString styleSplitterHandle = R"(
        QSplitter::handle:horizontal {
            width: 1px;
        }
        QSplitter::handle:vertical {
            height: 1px;
        }
)";

QString styleToolBar = R"(
        QToolBar {
            border: 0px;
            padding: 1px 1px;
        }
        QToolBar > QToolButton {
            border-radius: 3px;
            padding: 4px 4px;
            height: 16px;
            width: 16px;
            max-width: 16px;
            max-height: 16px;
        }
        QToolBar > QToolButton:hover {
            background-color: lightGray;
            border-color: auto;
        }
        QToolBar > QToolButton:pressed {
            background-color: darkGray;
            border-color: auto;
        }
        QToolBar > QToolButton:checked {
            background-color: darkGray;
            border-color: auto;
        }
)";

