TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    eDoc-Configuration \
    eDoc-API \
    eDoc-MetadataFramework \
    eDoc-Factory \
    sqlmanager \
    eDoc-CORE \
    eDoc-ClientComponents \
    eDoc-Client \
    testCORE \
    SimpleFileEngine \
    MemoryDocEngine \
    InMemoryDatabasePlugin \
    GenericDatabasePlugin \
    eDoc-InMemoryTagging \
    eDocTCPMessages \
    eDoc-tcpClient/edoctcpclient.pro \
    eDoc-Console \
#     tcpMessages
    eDocTcpServerPlugin/eDocTcpServerPlugin.pro \
    HistoricDatabasePlugin \
    eDocTcpHistoricClient \
    eDoc-SimpleFileIndexerEngine \
    eDocJsonServerPlugin


