#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T10:58:48
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eDoc-Factory
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCFACTORY_LIBRARY

SOURCES += edocfactory.cpp \
    configreader.cpp

HEADERS += edocfactory.h\
        edoc-factory_global.h \
    configreader.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/release/ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../eDoc-API/debug/ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API

include(../Logging/QsLog.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Configuration
DEPENDPATH += $$PWD/../eDoc-Configuration
