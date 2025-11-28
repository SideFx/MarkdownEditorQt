/////////////////////////////////////////////////////////////////////////////
// Name:        conversions.h
// Purpose:     Simple conversions
// Author:      Jan Buchholz
// Created:     2025-11-23
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QByteArray>
#include <QString>

inline QString ByteArrayToString(QByteArray ba) {
    return QString::fromUtf8(ba);
}

inline QByteArray StringToByteArray(QString str) {
    return str.toUtf8();
}
