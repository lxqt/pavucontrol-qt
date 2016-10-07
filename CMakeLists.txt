cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(pavucontrol-qt)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(LXQTBT_MINIMUM_VERSION "0.1.0")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

find_package(Qt5Widgets REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)
find_package(Qt5DBus REQUIRED)

find_package(lxqt-build-tools ${LXQTBT_MINIMUM_VERSION} REQUIRED)

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
)

add_subdirectory(src)

