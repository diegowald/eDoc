#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T16:50:24
#
#-------------------------------------------------

QT       += widgets

TARGET = eDoc-ClientComponents
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCCLIENTCOMPONENTS_LIBRARY

SOURCES += edocclientcomponents.cpp

HEADERS += edocclientcomponents.h\
        edoc-clientcomponents_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
