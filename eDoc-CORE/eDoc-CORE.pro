#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T18:32:52
#
#-------------------------------------------------

QT       += sql core

QT       -= gui

TARGET = edoccore
TEMPLATE = lib
CONFIG += plugin
#CONFIG += staticlib
DESTDIR = ../plugins

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

DEFINES += EDOCCORE_LIBRARY

SOURCES += edoccore.cpp \
    docengine.cpp \
    document.cpp \
    docid.cpp

HEADERS += edoccore.h\
    eDoc-CORE_global.h \
    docengine.h \
    document.h \
    docid.h \
    ../eDoc-API/iDocEngine.h \
    ../eDoc-API/iDocument.h \
    ../eDoc-API/iDocID.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
