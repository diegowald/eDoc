#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T18:32:52
#
#-------------------------------------------------

QT       += sql core

QT       -= gui

TARGET = eDoc-CORE
TEMPLATE = lib
#CONFIG += staticlib


DEFINES += EDOCCORE_LIBRARY

SOURCES += edoccore.cpp \
    docengine.cpp \
    document.cpp \
    docid.cpp

HEADERS += edoccore.h\
        eDoc-CORE_global.h \
    docengine.h \
    document.h \
    docid.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/debug/ -l"libeDoc-API.a"
else:unix: LIBS += -L$$OUT_PWD/../eDoc-API/ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API
