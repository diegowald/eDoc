#-------------------------------------------------
#
# Project created by QtCreator 2013-08-20T14:10:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = eDoc-Console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    task.cpp

include(../Logging/QsLog.pri)

HEADERS += \
    task.h

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Configuration
DEPENDPATH += $$PWD/../eDoc-Configuration

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Factory
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Factory
else:unix: LIBS += -L$$OUT_PWD/.. -leDoc-Factory

INCLUDEPATH += $$PWD/../eDoc-Factory
DEPENDPATH += $$PWD/../eDoc-Factory
