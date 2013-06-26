#-------------------------------------------------
#
# Project created by QtCreator 2013-06-26T09:27:56
#
#-------------------------------------------------

QT       -= gui

TARGET = eDoc-API
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCAPI_LIBRARY

SOURCES += edocapi.cpp

HEADERS += edocapi_global.h \
        IDocID.h \
        IDocument.h \
        IDocEngine.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
