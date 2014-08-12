#-------------------------------------------------
#
# Project created by QtCreator 2013-07-07T20:53:40
#
#-------------------------------------------------

QT       += core sql

TARGET = $$qtLibraryTarget(GenericDatabasePlugin)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins

CONFIG += c++11

win32 {
INCLUDEPATH += c:/boost/boost_1_54_0
}

SOURCES += \
    genericdatabase.cpp \
    recordid.cpp

HEADERS += \
    genericdatabase.h \
    recordid.h
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


unix|win32: LIBS += -L$$OUT_PWD/.. -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework

OTHER_FILES += \
    GenericDatabasePlugin.json

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lsqlmanager
else:unix: LIBS += -L$$OUT_PWD/../ -lsqlmanager


INCLUDEPATH += $$PWD/../sqlmanager
DEPENDPATH += $$PWD/../sqlmanager
