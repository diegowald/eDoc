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

CONFIG += c++11

SOURCES += main.cpp\
    dlgadddocument.cpp \
    explorerwindow.cpp \
    setdatetimetouse.cpp

HEADERS  += \
    dlgadddocument.h \
    explorerwindow.h \
    setdatetimetouse.h

FORMS    += \
    dlgadddocument.ui \
    explorerwindow.ui \
    setdatetimetouse.ui

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Factory
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Factory
else:unix: LIBS += -L$$OUT_PWD/.. -leDoc-Factory

INCLUDEPATH += $$PWD/../eDoc-Factory
DEPENDPATH += $$PWD/../eDoc-Factory

include(../Logging/QsLog.pri)

OTHER_FILES += \
    client.conf.xml \
    ../../edoc-bin/samantha.client.conf.xml

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Configuration
DEPENDPATH += $$PWD/../eDoc-Configuration

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-ClientComponents

INCLUDEPATH += $$PWD/../eDoc-ClientComponents
DEPENDPATH += $$PWD/../eDoc-ClientComponents

RESOURCES += \
    eDoc-Client.qrc
