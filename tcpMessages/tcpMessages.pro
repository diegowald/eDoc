#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T19:32:33
#
#-------------------------------------------------

QT       += core network

TARGET = tcpMessages
TEMPLATE = lib
DESTDIR = ../

DEFINES += TCPMESSAGES_LIBRARY

SOURCES += tcpmessages.cpp \
    messagebase.cpp \
    tcpadddocumentrequest.cpp \
    adddocumentresponse.cpp \
    tcpcommunicator.cpp \
    getblobrequest.cpp \
    getblobresponse.cpp \
    tcpchunck.cpp

HEADERS += tcpmessages.h\
        tcpmessages_global.h \
    messageBase.h \
    tcpAddDocumentRequest.h \
    adddocumentresponse.h \
    tcpcommunicator.h \
    getblobrequest.h \
    getblobresponse.h \
    tcpchunck.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix|win32: LIBS += -L$$OUT_PWD/../ -leDoc-API

INCLUDEPATH += $$PWD/../eDoc-API
DEPENDPATH += $$PWD/../eDoc-API
