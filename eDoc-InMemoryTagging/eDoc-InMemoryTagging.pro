#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T09:47:31
#
#-------------------------------------------------
QT       += sql
QT       -= gui

TARGET = eDoc-InMemoryTagging
TEMPLATE = lib
DESTDIR = ../plugins

DEFINES += EDOCINMEMORYTAGGING_LIBRARY

CONFIG += c++11

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

OTHER_FILES += \
    TagProcessorPlugin.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager


INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager
