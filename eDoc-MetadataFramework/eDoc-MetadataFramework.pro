#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T09:04:56
#
#-------------------------------------------------

QT       -= gui

TARGET = eDoc-MetadataFramework
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCMETADATAFRAMEWORK_LIBRARY

CONFIG += c++11

SOURCES += edocmetadataframework.cpp \
    valuedefinitions.cpp \
    fielddefinition.cpp \
    record.cpp \
    tag.cpp \
    parameter.cpp

HEADERS += edocmetadataframework.h\
    edoc-metadataframework_global.h \
    value.h \
    valuedefinitions.h \
    fielddefinition.h \
    record.h \
    tag.h \
    parameter.h

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
