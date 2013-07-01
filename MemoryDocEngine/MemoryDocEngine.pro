#-------------------------------------------------
#
# Project created by QtCreator 2013-06-30T17:47:25
#
#-------------------------------------------------

QT       += core
QT       += widgets


TARGET = $$qtLibraryTarget(MemoryDocEngine)
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins

SOURCES += memorydocengine.cpp \
    inmemorydocument.cpp

HEADERS += memorydocengine.h \
    inmemorydocument.h
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

OTHER_FILES += \
    MemoryDocEngine.json
