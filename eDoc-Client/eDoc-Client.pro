#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T17:58:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eDoc-Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../eDoc-API/ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-CORE/ -leDoc-CORE
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-CORE/ -leDoc-CORE
else:unix: LIBS += -L$$OUT_PWD/../eDoc-CORE/ -leDoc-CORE

INCLUDEPATH += $$PWD/../eDoc-CORE
DEPENDPATH += $$PWD/../eDoc-CORE
