TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    eDoc-Configuration \
    eDoc-API \
    eDoc-MetadataFramework \
    eDoc-Factory \
    sqlmanager \
    eDoc-CORE \
    eDoc-Client \
    testCORE \
    SimpleFileEngine \
    MemoryDocEngine \
    InMemoryDatabasePlugin \
    GenericDatabasePlugin \
    eDoc-ClientComponents \
    eDoc-InMemoryTagging \
    eDocTCPMessages \
    eDoc-tcpClient/edoctcpclient.pro \
    eDoc-Console \
#     tcpMessages
    eDocTcpServerPlugin/eDocTcpServerPlugin.pro \
    HistoricDatabasePlugin \
    eDocTcpHistoricClient


