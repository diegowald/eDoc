#-------------------------------------------------
#
# Project created by QtCreator 2014-08-24T12:31:49
#
#-------------------------------------------------

QT       += sql core network
QT       -= gui

TARGET = $$qtLibraryTarget(eDocTcpHistoricClient)
TEMPLATE = lib
DESTDIR = ../plugins
CONFIG += plugin
CONFIG += c++11

DEFINES += EDOCTCPHISTORICCLIENT_LIBRARY

SOURCES += edoctcphistoricclient.cpp

HEADERS += edoctcphistoricclient.h\
        edoctcphistoricclient_global.h


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    eDocTcpHistoricClient.json

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
