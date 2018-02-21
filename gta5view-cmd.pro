#/*****************************************************************************
#* gta5view-cmd - gta5view Command Line
#* Copyright (C) 2017-2018 Syping
#*
#* This program is free software: you can redistribute it and/or modify
#* it under the terms of the GNU General Public License as published by
#* the Free Software Foundation, either version 3 of the License, or
#* (at your option) any later version.
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program.  If not, see <http://www.gnu.org/licenses/>.
#*****************************************************************************/

QT += gui

DEPLOYMENT.display_name = gta5view-cmd
TARGET = gta5view-cmd
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp \
    gta5view/SnapmaticPicture.cpp \
    gta5view/StringParser.cpp

HEADERS  += \
    gta5view/SnapmaticPicture.h \
    gta5view/StringParser.h

INCLUDEPATH += ./gta5view

# WINDOWS ONLY

win32: DEFINES += GTA5SYNC_WIN
# win32: RC_FILE += res/app.rc
win32: CONFIG -= embed_manifest_exe

RESOURCES += \
    res.qrc

isEmpty(GTA5SYNC_PREFIX): GTA5SYNC_PREFIX = /usr/local
target.path = $$GTA5SYNC_PREFIX/bin
INSTALLS += target
