#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T16:50:24
#
#-------------------------------------------------

QT       += widgets core gui

TARGET = eDoc-ClientComponents
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCCLIENTCOMPONENTS_LIBRARY

SOURCES += edocclientcomponents.cpp \
    stringwidget.cpp \
    recordeditor.cpp

HEADERS += edocclientcomponents.h\
        edoc-clientcomponents_global.h \
    stringwidget.h \
    recordeditor.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    stringwidget.ui \
    recordeditor.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-API
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API
