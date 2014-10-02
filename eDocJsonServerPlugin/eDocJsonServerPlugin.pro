#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T15:20:45
#
#-------------------------------------------------

QT       += network
QT       -= gui

TARGET = eDocJsonServerPlugin
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../plugins

DEFINES += EDOCJSONSERVERPLUGIN_LIBRARY

SOURCES += edocjsonserverplugin.cpp \
    jsonserver.cpp \
    jsonprocessorthread.cpp \
    jsonhelpers.cpp

HEADERS += edocjsonserverplugin.h\
        edocjsonserverplugin_global.h \
    jsonserver.h \
    jsonprocessorthread.h \
    jsonhelpers.h

OTHER_FILES += eDocJsonServerPlugin.json \
    eDocJsonServerPlugin.json

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework
