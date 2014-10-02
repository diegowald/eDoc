#-------------------------------------------------
#
# Project created by QtCreator 2014-08-12T09:42:17
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = sqlmanager
TEMPLATE = lib
DESTDIR = ../

CONFIG += c++11

DEFINES += SQLMANAGER_LIBRARY

SOURCES += sqlmanager.cpp

HEADERS += sqlmanager.h\
        sqlmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Cofniguration
DEPENDPATH += $$PWD/../eDoc-Configuration
