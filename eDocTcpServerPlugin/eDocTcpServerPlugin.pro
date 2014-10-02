#-------------------------------------------------
#
# Project created by QtCreator 2014-08-19T14:22:47
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = eDocTcpServerPlugin
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../plugins

DEFINES += EDOCTCPSERVERPLUGIN_LIBRARY

SOURCES += edoctcpserverdatabaseplugin.cpp \
    tcpserver.cpp

HEADERS += edoctcpserverdatabaseplugin.h \
    edoctcpserverplugin_global.h \
    tcpserver.h

OTHER_FILES += eDocTcpServerPlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager

INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager

unix|win32: LIBS += -L$$OUT_PWD/../ -leDocTCPMessages

INCLUDEPATH += $$PWD/../eDocTCPMessages
DEPENDPATH += $$PWD/../eDocTCPMessages

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Cofniguration
DEPENDPATH += $$PWD/../eDoc-Configuration

unix|win32: LIBS += -L$$OUT_PWD/.. -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework
