#-------------------------------------------------
#
# Project created by QtCreator 2013-06-25T10:58:48
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eDoc-Factory
TEMPLATE = lib

DEFINES += EDOCFACTORY_LIBRARY

SOURCES += edocfactory.cpp

HEADERS += edocfactory.h\
        edoc-factory_global.h

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
else:unix: LIBS += -L$$OUT_PWD/../eDoc-API/ -l"eDoc-API"

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API
