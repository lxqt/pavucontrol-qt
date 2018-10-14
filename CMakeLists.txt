project(pavucontrol-qt)
# CMP0000 - A minimum required CMake version must be specified.
# Note that the command invocation must appear in the CMakeLists.txt
# file itself; a call in an included file is not sufficient.
cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

set(LXQTBT_MINIMUM_VERSION "0.5.0")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5DBus REQUIRED)

find_package(lxqt-build-tools ${LXQTBT_MINIMUM_VERSION} REQUIRED)

set(PAVUCONTROLQT_MAJOR_VERSION 0)
set(PAVUCONTROLQT_MINOR_VERSION 4)
set(PAVUCONTROLQT_PATCH_VERSION 0)
set(PAVUCONTROLQT_VERSION ${PAVUCONTROLQT_MAJOR_VERSION}.${PAVUCONTROLQT_MINOR_VERSION}.${PAVUCONTROLQT_PATCH_VERSION})
add_definitions("-DPAVUCONTROLQT_VERSION=\"${PAVUCONTROLQT_VERSION}\"")

include(LXQtPreventInSourceBuilds)
include(LXQtCompilerSettings NO_POLICY_SCOPE)
include(LXQtTranslate)

find_package(PkgConfig REQUIRED)
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
