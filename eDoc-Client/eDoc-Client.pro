#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T17:58:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eDocClient
TEMPLATE = app
DESTDIR = ../

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-Factory/release/ -leDoc-Factory
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-Factory/debug/ -leDoc-Factory
else:unix: LIBS += -L$$OUT_PWD/../eDoc-Factory/ -leDoc-Factory

INCLUDEPATH += $$PWD/../eDoc-Factory
DEPENDPATH += $$PWD/../eDoc-Factory

include(../Logging/QsLog.pri)
