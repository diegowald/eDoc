#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T09:47:31
#
#-------------------------------------------------

QT       -= gui

TARGET = eDoc-InMemoryTagging
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCINMEMORYTAGGING_LIBRARY

SOURCES += \
    inmemorytagprocessor.cpp

HEADERS +=\
        edoc-inmemorytagging_global.h \
    inmemorytagprocessor.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
