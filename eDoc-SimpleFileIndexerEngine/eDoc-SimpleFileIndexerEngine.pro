#-------------------------------------------------
#
# Project created by QtCreator 2014-09-10T17:33:34
#
#-------------------------------------------------

QT       -= gui
QT       += core sql widgets

TARGET = $$qtLibraryTarget(eDoc-SimpleFileIndexerEngine)
TEMPLATE = lib
CONFIG  += plugin
CONFIG  += c++11
DESTDIR = ../plugins
DEFINES += EDOCSIMPLEFILEINDEXERENGINE_LIBRARY

SOURCES += edocsimplefileindexerengine.cpp

HEADERS += edocsimplefileindexerengine.h\
        edoc-simplefileindexerengine_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

include(../Logging/QsLog.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager


INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager

OTHER_FILES += \
    SimpleFileIndexerEngine.json

