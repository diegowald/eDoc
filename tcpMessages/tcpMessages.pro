#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T19:32:33
#
#-------------------------------------------------

QT       += core

TARGET = tcpMessages
TEMPLATE = lib
DESTDIR = ../

DEFINES += TCPMESSAGES_LIBRARY

SOURCES += tcpmessages.cpp \
    messagebase.cpp \
    tcpadddocumentrequest.cpp

HEADERS += tcpmessages.h\
        tcpmessages_global.h \
    messageBase.h \
    tcpAddDocumentRequest.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
