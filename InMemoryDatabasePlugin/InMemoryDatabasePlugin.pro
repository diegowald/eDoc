#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T13:45:02
#
#-------------------------------------------------

QT       += core

TARGET = $$qtLibraryTarget(InMemoryDatabasePlugin)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins


SOURCES += \
    inmemorydatabase.cpp \
    recordid.cpp \
    record.cpp

HEADERS += \
    inmemorydatabase.h \
    recordid.h \
    record.h
OTHER_FILES += InMemoryDatabasePlugin.json

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


unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework
