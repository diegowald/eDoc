#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T14:41:31
#
#-------------------------------------------------
QT       += sql core network
QT       -= gui

TARGET = $$qtLibraryTarget(eDocTcpClient)
TEMPLATE = lib
DESTDIR = ../plugins

DEFINES += EDOCTCPCLIENT_LIBRARY
CONFIG += plugin
CONFIG += c++11

SOURCES += \
    edoctcpclientdatabaseplugin.cpp

HEADERS += \
        edoctcpclient_global.h \
    edoctcpclientdatabaseplugin.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    TagProcessorPlugin.json \
    eDocTcpClient.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager


INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager

unix|win32: LIBS += -L$$OUT_PWD/../ -leDocTCPMessages

INCLUDEPATH += $$PWD/../eDocTCPMessages
DEPENDPATH += $$PWD/../eDocTCPMessages
