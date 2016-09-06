# pavucontrol-qt

## Overview

pavucontrol-qt is the Qt port of volume control [pavucontrol](https://freedesktop.org/software/pulseaudio/pavucontrol/) of sound server [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/).   

As such it can be used to adjust all controls provided by PulseAudio as well as some additional settings.   

The software belongs to the LXQt project but its usage isn't limited to this desktop environment.   

## Installation

### Compiling source code

Runtime dependencies are qtbase and PulseAudio client library libpulse.   
Additional build dependencies are CMake, xdg-user-dirs and [liblxqt](https://github.com/lxde/liblxqt) as well as optionally Git to pull latest VCS checkouts. The localization files were outsourced to repository [lxqt-l10n](https://github.com/lxde/lxqt-l10n) so the corresponding dependencies are needed, too. Please refer to this repository's `README.md` for further information.   

Code configuration is handled by CMake. CMake variable `CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.   

To build run `make`, to install `make install` which accepts variable `DESTDIR` as usual.   

### Binary packages

This project was launched in August 2016 and binary packages are rare so far. On Arch Linux an [AUR](https://aur.archlinux.org/) package [pavucontrol-qt-git](https://aur.archlinux.org/packages/pavucontrol-qt-git/) can be used to build current checkouts of branch `master`.

## Usage

In LXQt sessions the binary is placed in sub-menu "Sound & Video" of the panel's main menu.   

The usage itself should be self-explanatory.   
