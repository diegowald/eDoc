#-------------------------------------------------
#
# Project created by QtCreator 2013-08-16T11:22:10
#
#-------------------------------------------------

QT       += core network

TARGET = $$qtLibraryTarget(eDoc-tcpClient)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins


SOURCES += \
    tcpclientplugin.cpp

HEADERS += \
    tcpclientplugin.h
OTHER_FILES += eDoc-HttpClient.json

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/.. -ltcpMessages
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/.. -ltcpMessages
else:unix: LIBS += -L$$OUT_PWD/.. -ltcpMessages

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../tcpMessages
