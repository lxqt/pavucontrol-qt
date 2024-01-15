cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(pavucontrol-qt)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)

# Enable Qt6 build preference
option (CONFIG_QT6 "Enable Qt6 build (default=yes)" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

# Minimum Versions
set(GLIB_MINIMUM_VERSION "2.50.0")
set(LXQTBT_MINIMUM_VERSION "0.13.0")
if (CONFIG_QT6)
	set(QT_MINIMUM_VERSION "6.6.0")
	find_package(Qt6Widgets ${QT_MINIMUM_VERSION} REQUIRED)
	find_package(Qt6LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
	set(PAVUCONTROLQT_MAJOR_VERSION 2)
	set(PAVUCONTROLQT_MINOR_VERSION 0)
else()
	set(QT_MINIMUM_VERSION "5.15.0")
	find_package(Qt5Widgets ${QT_MINIMUM_VERSION} REQUIRED)
	find_package(Qt5LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
	set(PAVUCONTROLQT_MAJOR_VERSION 1)
	set(PAVUCONTROLQT_MINOR_VERSION 4)
endif()

set(PAVUCONTROLQT_PATCH_VERSION 0)

find_package(lxqt-build-tools ${LXQTBT_MINIMUM_VERSION} REQUIRED)

find_package(GLIB ${GLIB_MINIMUM_VERSION} REQUIRED)

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

add_subdirectory(src)
