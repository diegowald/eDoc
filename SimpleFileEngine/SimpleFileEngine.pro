#-------------------------------------------------
#
# Project created by QtCreator 2013-06-27T09:41:01
#
#-------------------------------------------------

QT       += core sql
QT       += widgets

TARGET = $$qtLibraryTarget(SimpleFileEngine)
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../plugins

SOURCES += fileengine.cpp

HEADERS += fileengine.h
OTHER_FILES += SimpleFileEngine.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/release/ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/debug/ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

include(../Logging/QsLog.pri)
