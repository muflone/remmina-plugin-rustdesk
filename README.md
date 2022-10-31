# Remmina Plugin RUSTDESK

[![Travis CI Build Status](https://img.shields.io/travis/com/muflone/remmina-plugin-rustdesk/master.svg)](https://www.travis-ci.com/github/muflone/remmina-plugin-rustdesk)
[![CircleCI Build Status](https://img.shields.io/circleci/project/github/muflone/remmina-plugin-rustdesk/master.svg)](https://circleci.com/gh/muflone/remmina-plugin-rustdesk)

**Description:** Remmina protocol plugin for Rustdesk connections.

**Copyright:** 2022 Fabio Castelli (Muflone) <muflone(at)muflone.com>

**License:** GPL-2+

**Source code:** https://github.com/muflone/remmina-plugin-rustdesk/

**Documentation**: http://www.muflone.com/remmina-plugin-rustdesk/

# Description

A [**Remmina**](https://github.com/freerdp/remmina) protocol plugin to connect
to a remote host using Rustdesk.

![Main window](http://www.muflone.com/resources/remmina-plugin-rustdesk/archive/latest/english/general.png)

# Install instructions

Download and extract [**Remmina Plugin Builder**](https://github.com/muflone/remmina-plugin-builder/releases/):

    wget -O remmina-plugin-builder.tar.gz https://github.com/muflone/remmina-plugin-builder/archive/1.4.27.0.tar.gz
    tar --extract --verbose --gzip --file remmina-plugin-builder.tar.gz

Copy the plugin source files to the **remmina-plugin-to-build** directory:

    cp --recursive remmina-plugin-rustdesk CMakeLists.txt remmina-plugin-builder-1.4.27.0/remmina-plugin-to-build/

Build the plugin using Remmina Plugin Builder:

    cd remmina-plugin-builder-1.4.27.0
    cmake -DCMAKE_INSTALL_PREFIX=/usr .
    make

Install the plugin into the Remmina plugins directory (may need sudo or root
access):

    sudo make install

You'll find it in the remmina connection editor.

# License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
