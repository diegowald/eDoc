#-------------------------------------------------
#
# Project created by QtCreator 2013-08-20T14:10:58
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = eDocConsole
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = ../
TEMPLATE = app


SOURCES += main.cpp \
    task.cpp \
    tcpcommunicator.cpp

include(../Logging/QsLog.pri)

HEADERS += \
    task.h \
    tcpcommunicator.h

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