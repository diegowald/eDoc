#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T18:01:25
#
#-------------------------------------------------

QT       -= gui

TARGET = eDoc-API
TEMPLATE = lib

DEFINES += EDOCAPI_LIBRARY

SOURCES += edocapi.cpp

HEADERS += edocapi.h\
        eDoc-API_global.h \
    IDocEngine.h \
    IDocID.h \
    IDocument.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
