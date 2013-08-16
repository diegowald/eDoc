#-------------------------------------------------
#
# Project created by QtCreator 2013-08-16T11:22:10
#
#-------------------------------------------------

QT       += core

TARGET = $$qtLibraryTarget(eDoc-HttpClient)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins


SOURCES += httpclientplugin.cpp

HEADERS += httpclientplugin.h
OTHER_FILES += eDoc-HttpClient.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API
