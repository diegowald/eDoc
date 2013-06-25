#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T10:58:48
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eDoc-Factory
TEMPLATE = lib

DEFINES += EDOCFACTORY_LIBRARY

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

SOURCES += edocfactory.cpp

HEADERS += edocfactory.h\
        edoc-factory_global.h \
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
