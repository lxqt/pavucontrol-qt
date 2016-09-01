if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(
    ${PULSE_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

set(pavucontrol-qt_HDRS
    pavucontrol.h
    mainwindow.h
    cardwidget.h
    channelwidget.h
    devicewidget.h
    minimalstreamwidget.h
    rolewidget.h
    sinkinputwidget.h
    sinkwidget.h
    sourceoutputwidget.h
    sourcewidget.h
    streamwidget.h
)

set(pavucontrol-qt_SRCS
    pavucontrol.cc
    mainwindow.cc
    cardwidget.cc
    channelwidget.cc
    devicewidget.cc
    minimalstreamwidget.cc
    rolewidget.cc
    sinkinputwidget.cc
    sinkwidget.cc
    sourceoutputwidget.cc
    sourcewidget.cc
    streamwidget.cc
)

set(pavucontrol-qt_UI
    cardwidget.ui
    channelwidget.ui
    devicewidget.ui
    mainwindow.ui
    streamwidget.ui
)

# Translations
lxqt_translate_ts(pavucontrol-qt_QM_FILES
    UPDATE_TRANSLATIONS
        ${UPDATE_TRANSLATIONS}
    SOURCES
        ${pavucontrol-qt_HDRS}
        ${pavucontrol-qt_SRCS}
        ${pavucontrol-qt_UI}
    INSTALL_DIR
        "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/translations"
    PULL_TRANSLATIONS
        ${PULL_TRANSLATIONS}
    CLEAN_TRANSLATIONS
        ${CLEAN_TRANSLATIONS}
    TRANSLATIONS_REPO
        ${TRANSLATIONS_REPO}
    TRANSLATIONS_REFSPEC
        ${TRANSLATIONS_REFSPEC}
)
lxqt_translate_desktop(DESKTOP_FILES
    SOURCES
    pavucontrol-qt.desktop.in
)

add_executable(pavucontrol-qt
    ${pavucontrol-qt_SRCS}
    ${pavucontrol-qt_QM_FILES}
    ${pavucontrol-qt_QM_LOADER}
    ${DESKTOP_FILES}
)
set_property(
     TARGET pavucontrol-qt APPEND
     PROPERTY COMPILE_DEFINITIONS
     PAVUCONTROL_QT_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}"
)

target_link_libraries(pavucontrol-qt
    Qt5::Widgets
    Qt5::DBus
    ${PULSE_LIBRARIES}
    ${GLIB_LIBRARIES}
)

install(TARGETS
    pavucontrol-qt
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)

install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    COMPONENT Runtime
)
