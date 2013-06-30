#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T11:17:02
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = eDoc-Configuration
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCCONFIGURATION_LIBRARY

SOURCES += edocconfiguration.cpp \
        xmlcollection.cpp \
        xmlelement.cpp \
    qobjectlgging.cpp

HEADERS += edocconfiguration.h\
        edoc-configuration_global.h \
        IXMLContent.h \
        xmlcollection.h \
        xmlelement.h \
    qobjectlgging.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
