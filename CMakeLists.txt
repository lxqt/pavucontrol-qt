cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(pavucontrol-qt)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)

find_package(lxqt REQUIRED QUIET)
find_package(XdgUserDirs REQUIRED QUIET)

include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslate)

find_package(PkgConfig REQUIRED QUIET)
pkg_check_modules(
    PULSE REQUIRED
    libpulse>=5.0
    libpulse-mainloop-glib>=0.9.16
)
pkg_check_modules(
    GLIB REQUIRED
    glib-2.0
    glibmm-2.4  # c++ binding for glib
)

add_subdirectory(src)

