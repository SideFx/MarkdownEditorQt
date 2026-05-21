cmake_minimum_required(VERSION 3.25)

project(MarkdownEditorQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Svg)

add_subdirectory("${CMAKE_SOURCE_DIR}/md4c")

qt_add_executable(MarkdownEditorQt
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    stylesheets.h
    constants.h
    jbpreferences.cpp jbpreferences.h
    resources.qrc
    dialogs.h dialogs.cpp
    io.h io.cpp
    aboutdialog.h aboutdialog.cpp aboutdialog.ui
    scrollsynchronizer.h scrollsynchronizer.cpp
    conversions.hpp
    fixedsplitter.hpp
    imagehandler.h imagehandler.cpp
    synchelper.h synchelper.cpp
    app.icns app.ico appicon.rc
)

target_link_libraries(MarkdownEditorQt PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Svg
    md4c
    md4c-html
)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE app.icns)
    set(APP_ICON "${CMAKE_SOURCE_DIR}/app.icns")
    set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

# ---- Bundle Properties ----
set_target_properties(MarkdownEditorQt PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# ---- Installation ----
include(GNUInstallDirs)
install(TARGETS MarkdownEditorQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- macOS Deployment ----
if(APPLE)
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)
    find_program(MACDEPLOYQT macdeployqt6 HINTS "${QT_BIN_DIR}")
    add_custom_command(TARGET MarkdownEditorQt POST_BUILD
        COMMAND ${MACDEPLOYQT} "$<TARGET_BUNDLE_DIR:MarkdownEditorQt>" -verbose=2
    )
    add_custom_command(TARGET MarkdownEditorQt POST_BUILD
        COMMAND codesign --force --deep --sign - --timestamp=none "$<TARGET_BUNDLE_DIR:MarkdownEditorQt>"
    )
endif()


