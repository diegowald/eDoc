#-------------------------------------------------
#
# Project created by QtCreator 2013-06-26T09:27:56
#
#-------------------------------------------------

QT       -= gui

TARGET = eDoc-API
TEMPLATE = lib
DESTDIR = ../

DEFINES += EDOCAPI_LIBRARY

SOURCES += edocapi.cpp

HEADERS += edocapi_global.h \
        IDocID.h \
        IDocument.h \
        IDocEngine.h \
    IDatabase.h \
    IFieldDefinition.h \
    IRecord.h \
    IParameter.h \
    APIEnums.h \
    IValue.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -leDoc-Configuration
else:unix: LIBS += -L$$OUT_PWD/../ -leDoc-Configuration

INCLUDEPATH += $$PWD/../eDoc-Configuration
DEPENDPATH += $$PWD/../eDoc-Configuration
