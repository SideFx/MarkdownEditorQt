cmake_minimum_required(VERSION 3.25)

project(MarkdownEditorQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)
add_subdirectory("${CMAKE_SOURCE_DIR}/md4c")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network Svg)

set(QM_FILES ${CMAKE_SOURCE_DIR}/translations/qt_en.qm)

if(APPLE)
set(MACOSX_BUNDLE_ICON_FILE app.icns)
set(APP_ICON "${CMAKE_SOURCE_DIR}/app.icns")
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

    qt_add_executable(MarkdownEditorQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        stylesheets.h
        constants.h
        jbpreferences.cpp jbpreferences.h
        assets/mainTB_Font.svg
        assets/mainTB_Open.svg
        assets/mainTB_Quit.svg
        assets/mainTB_Save.svg
        assets/mainTB_Sync.svg
        assets/mainTB_SaveAs.svg
        resources.qrc
        assets/mainTB_New.svg
        assets/mainTB_Copy.svg
        assets/mainTB_Cut.svg
        assets/mainTB_Paste.svg
        assets/mainTB_Redo.svg
        assets/mainTB_SelectAll.svg
        assets/mainTB_Undo.svg
        assets/mainTB_Info.svg
        assets/mainTB_Settings.svg
        dialogs.h dialogs.cpp
        assets/mainTB_Quit_2.svg
        assets/mainTB_Edit.svg
        assets/mainTB_Edit_2.svg
        assets/mainTB_Key.svg
        app.icns app.ico appicon.rc
        appicon.rc
        io.h io.cpp
        aboutdialog.h aboutdialog.cpp aboutdialog.ui
        scrollsynchronizer.h scrollsynchronizer.cpp
        conversions.hpp
        fixedsplitter.hpp
        assets/logo.png
        assets/mainTB_FormatBold.svg assets/mainTB_FormatItalic.svg assets/mainTB_FormatUnderlined.svg
        assets/mainTB_FormatCode.svg
        assets/mainTB_FormatHighlight.svg
        assets/mainTB_FormatStrikethrough.svg
        imagehandler.h imagehandler.cpp
        synchelper.h synchelper.cpp
    )
endif()

target_link_libraries(MarkdownEditorQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(MarkdownEditorQt PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Svg)
target_link_libraries(MarkdownEditorQt PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(MarkdownEditorQt PRIVATE md4c md4c-html)

set_target_properties(MarkdownEditorQt PROPERTIES
    ${BUNDLE_ID_OPTION}
#    UPDATE_TRANSLATIONS .
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MarkdownEditorQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MarkdownEditorQt)
endif()

# --- Windows ---
if(WIN32)
    set(QM_DEST ${CMAKE_BINARY_DIR}/translations)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
endif()

# --- macOS ---
if(APPLE)
    # app bundle
    set(QM_DEST ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/i18n)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
endif()

# --- Linux/Unix ---
if(UNIX AND NOT APPLE)
    # local
    set(QM_DEST ${CMAKE_BINARY_DIR}/translations)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${QM_DEST}
    )
    # system
    install(FILES ${QM_FILES} DESTINATION share/${PROJECT_NAME}/translations)
endif()

