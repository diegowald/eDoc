#-------------------------------------------------
#
# Project created by QtCreator 2013-06-27T09:41:01
#
#-------------------------------------------------

QT       += core sql
QT       += widgets

TARGET = $$qtLibraryTarget(SimpleFileEngine)
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../plugins

CONFIG += c++11

SOURCES += fileengine.cpp \
    simplefileid.cpp \
    simplefiledocument.cpp \
    filemanagement.cpp

HEADERS += fileengine.h \
    simplefileid.h \
    simplefiledocument.h \
    filemanagement.h
OTHER_FILES += SimpleFileEngine.json

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Cofniguration
DEPENDPATH += $$PWD/../eDoc-Configuration

include(../Logging/QsLog.pri)
