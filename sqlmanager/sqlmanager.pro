#-------------------------------------------------
#
# Project created by QtCreator 2014-08-12T09:42:17
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = sqlmanager
TEMPLATE = lib
DESTDIR = ../

CONFIG += c++11

DEFINES += SQLMANAGER_LIBRARY

SOURCES += sqlmanager.cpp

HEADERS += sqlmanager.h\
        sqlmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
