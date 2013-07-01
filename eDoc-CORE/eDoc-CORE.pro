#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T18:32:52
#
#-------------------------------------------------

QT       += sql core
QT       += widgets

#TARGET = edoccore
TARGET  = $$qtLibraryTarget(edoccore)
TEMPLATE = lib
CONFIG += plugin
#CONFIG += staticlib
DESTDIR = ../plugins

DEFINES += EDOCCORE_LIBRARY

SOURCES += docengine.cpp \
    #edoccore.cpp \
    document.cpp \
    docid.cpp \
    main.cpp

HEADERS += docengine.h \
    eDoc-CORE_global.h \
    #edoccore.h\
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

include(../Logging/QsLog.pri)


EXAMPLE_FILES = echoplugin.json
OTHER_FILES += \
    DocEngine.json
