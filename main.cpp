/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     Main
// Author:      Jan Buchholz
// Created:     2025-11-23
// Changed:     2026-11-26
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "markdownapplication.h"
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QString filePath{};
    MarkdownApplication app(argc, argv);
    const QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        filePath = args.at(1);
#if !defined (Q_OS_MACOS)
        // Instance already running?
        if (app.sendFileToRunningInstance(filePath)) return 0;
#endif
    }
    app.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    app.setMainWindow(&w);
    w.show();
#if !defined (Q_OS_MACOS)
    app.startLocalServer(&w);
#endif
    if (!filePath.isEmpty()) {
        w.openFile(filePath);
    }
    return app.exec();
}
