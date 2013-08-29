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

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-Factory

INCLUDEPATH += $$PWD/../eDoc-Factory
DEPENDPATH += $$PWD/../eDoc-Factory

unix|win32: LIBS += -L$$OUT_PWD/../ -ltcpMessages

INCLUDEPATH += $$PWD/../tcpMessages
DEPENDPATH += $$PWD/../tcpMessages
