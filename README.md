# pavucontrol-qt

## Overview

pavucontrol-qt is the Qt port of the volume control [pavucontrol](https://freedesktop.org/software/pulseaudio/pavucontrol/) for the sound server [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/).   

As such it can be used to adjust all controls provided by PulseAudio and ALSA as well as some additional settings.   

The software belongs to the LXQt project but its usage isn't limited to this desktop environment.   

## Installation

### Compiling source code

Runtime dependencies are qtbase and PulseAudio client library libpulse.   
Additional build dependencies are CMake and [liblxqt](https://github.com/lxqt/liblxqt) as well as optionally Git to pull latest VCS checkouts. 

Code configuration is handled by CMake. CMake variable `CMAKE_INSTALL_PREFIX` has to be set to `/usr` on most operating systems.   

To build run `make`, to install `make install` which accepts variable `DESTDIR` as usual.   

### Binary packages

Official binary packages are available in Arch Linux, Debian, Fedora and openSUSE (Leap and Tumbleweed) and most other distributions.

## Usage

In LXQt sessions the binary is placed in sub-menu "Sound & Video" of the panel's main menu.   

The usage itself should be self-explanatory.


## Translations

Translations can be done in [LXQt-Weblate](https://translate.lxqt-project.org/projects/lxqt-configuration/pavucontrol-qt/).

<a href="https://translate.lxqt-project.org/projects/lxqt-configuration/pavucontrol-qt/">
<img src="https://translate.lxqt-project.org/widgets/lxqt-configuration/-/pavucontrol-qt/multi-auto.svg" alt="Translation status" />
</a>
