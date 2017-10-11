#/*****************************************************************************
#* gta5view-cmd - gta5view Command Line
#* Copyright (C) 2017 Syping
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
    gta5sync/SnapmaticPicture.cpp \
    gta5sync/StringParser.cpp

HEADERS  += \
    gta5sync/SnapmaticPicture.h \
    gta5sync/StringParser.h

INCLUDEPATH += ./gta5sync

# WINDOWS ONLY

win32: DEFINES += GTA5SYNC_WIN
# win32: RC_FILE += res/app.rc
win32: LIBS += -luser32
win32: CONFIG -= embed_manifest_exe

# QT4 ONLY STUFF

isEqual(QT_MAJOR_VERSION, 4): INCLUDEPATH += ./gta5sync/qjson4
isEqual(QT_MAJOR_VERSION, 4): HEADERS += gta5sync/qjson4/QJsonArray.h \
    gta5sync/qjson4/QJsonDocument.h \
    gta5sync/qjson4/QJsonObject.h \
    gta5sync/qjson4/QJsonParseError.h \
    gta5sync/qjson4/QJsonValue.h \
    gta5sync/qjson4/QJsonValueRef.h \
    gta5sync/qjson4/QJsonParser.h \
    gta5sync/qjson4/QJsonRoot.h

isEqual(QT_MAJOR_VERSION, 4): SOURCES += gta5sync/qjson4/QJsonArray.cpp \
    gta5sync/qjson4/QJsonDocument.cpp \
    gta5sync/qjson4/QJsonObject.cpp \
    gta5sync/qjson4/QJsonParseError.cpp \
    gta5sync/qjson4/QJsonValue.cpp \
    gta5sync/qjson4/QJsonValueRef.cpp \
    gta5sync/qjson4/QJsonParser.cpp

RESOURCES += \
    res.qrc
