#-------------------------------------------------
#
# Project created by QtCreator 2014-08-24T10:52:57
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET =$$qtLibraryTarget(HistoricDatabasePlugin)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins
CONFIG += c++11

DEFINES += HISTORICDATABASEPLUGIN_LIBRARY

SOURCES += \
    historicdatabase.cpp \
    ../GenericDatabasePlugin/recordid.cpp \
    historicrecord.cpp

HEADERS +=\
        historicdatabaseplugin_global.h \
    historicdatabase.h \
    ../GenericDatabasePlugin/recordid.h \
    historicrecord.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API


INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API


unix|win32: LIBS += -L$$OUT_PWD/.. -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework

OTHER_FILES += \
    GenericDatabasePlugin.json \
    HistoricDatabasePlugin.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager


INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager
