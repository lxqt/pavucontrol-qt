if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(
    ${PULSE_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

set(pavucontrol-qt_HDRS
    mainwindow.h
    cardwidget.h
)

set(pavucontrol-qt_SRCS
    pavucontrol.cc
    mainwindow.cc
    cardwidget.cc
)

set(pavucontrol-qt_UI
    src/wmselectdialog.ui
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
        "${LXQT_TRANSLATIONS_DIR}/${PROJECT_NAME}"
    # PULL_TRANSLATIONS
    #     ${PULL_TRANSLATIONS}
    CLEAN_TRANSLATIONS
        ${CLEAN_TRANSLATIONS}
    TRANSLATIONS_REPO
        ${TRANSLATIONS_REPO}
    TRANSLATIONS_REFSPEC
        ${TRANSLATIONS_REFSPEC}
    REPO_SUBDIR
        "pavucontrol-qt/${PROJECT_NAME}"
)
# lxqt_app_translation_loader(pavucontrol-qt_QM_LOADER ${PROJECT_NAME})

add_executable(pavucontrol-qt
    ${pavucontrol-qt_SRCS}
    ${pavucontrol-qt_QM_FILES}
    ${pavucontrol-qt_QM_LOADER}
)

target_link_libraries(pavucontrol-qt
    Qt5::Widgets
    Qt5::DBus
    ${PULSE_LDFLAGS}
    ${GLIB_LDFLAGS}
)

INSTALL(TARGETS
    pavucontrol-qt
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)
