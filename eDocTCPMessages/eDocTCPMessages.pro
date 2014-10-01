#-------------------------------------------------
#
# Project created by QtCreator 2014-08-20T08:21:58
#
#-------------------------------------------------
QT       += network core
QT       -= gui

TARGET = eDocTCPMessages
#TARGET = eDocTCPMessages1
TEMPLATE = lib
#TEMPLATE = app
DESTDIR = ../

DEFINES += EDOCTCPMESSAGES_LIBRARY
CONFIG += c++11

SOURCES += edoctcpmessages.cpp \
           header.cpp \
           proxyfielddefinition.cpp \
           proxyparameter.cpp \
           proxyrecord.cpp \
           proxyrecordid.cpp \
           proxyvalue.cpp \
           streamhelpers.cpp \
    messagecodes.cpp \
    proxydocid.cpp \
    tcpclient.cpp \
    proxydocumentvalue.cpp \
    proxydocument.cpp \
    main.cpp

HEADERS += edoctcpmessages.h\
           edoctcpmessages_global.h \
           header.h \
           proxyfielddefinition.h \
           proxyparameter.h \
           proxyrecord.h \
           proxyrecordid.h \
           proxyvalue.h \
           streamhelpers.h \
    messagecodes.h \
    proxydocid.h \
    tcpclient.h \
    proxydocumentvalue.h \
    proxydocument.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Configuration
DEPENDPATH += $$PWD/../eDoc-Configuration

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-MetadataFramework

INCLUDEPATH += $$PWD/../eDoc-MetadataFramework
DEPENDPATH += $$PWD/../eDoc-MetadataFramework
