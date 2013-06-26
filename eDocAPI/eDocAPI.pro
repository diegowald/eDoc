#-------------------------------------------------
#
# Project created by QtCreator 2013-06-26T09:27:56
#
#-------------------------------------------------

QT       -= gui

TARGET = eDocAPI
TEMPLATE = lib

DEFINES += EDOCAPI_LIBRARY

SOURCES += edocapi.cpp

HEADERS += edocapi.h\
        edocapi_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
