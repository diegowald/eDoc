#include "edoctcpserverdatabaseplugin.h"
#include <QPluginLoader>
#include <QtNetwork/QTcpSocket>

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDocTCPMessages/messagecodes.h"
#include "../eDocTCPMessages/streamhelpers.h"

EDocTCPServerDatabasePlugin::EDocTCPServerDatabasePlugin(QObjectLogging *Logger, QTcpSocket *socket, IDatabase *persistance, IDocEngine* docEngine, QObject *parent) :
    QThread(parent)
{
    _persistance = NULL;
    _socket = socket;
    _persistance = persistance;
    _docEngine = docEngine;
    blockSize = 0;
    blob.clear();
    buildingBlob.clear();
    out = NULL;
    logger = Logger;

    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));
    start();
}

EDocTCPServerDatabasePlugin::~EDocTCPServerDatabasePlugin()
{
    logger->logDebug("EDocTCPServerDatabasePlugin::~EDocTCPServerDatabasePlugin()");
}

void EDocTCPServerDatabasePlugin::run()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::run()");
    forever
    {
        if (_socket->state() == QAbstractSocket::ClosingState)
        {
            deleteLater();
        }

        _socket->waitForReadyRead();
        if (_socket->bytesAvailable() > 0)
        {
            onReadyRead();
        }
    }
}

void EDocTCPServerDatabasePlugin::onReadyRead()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::onReadyRead()");
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_3);

    if (blockSize == 0)
    {
        if (_socket->bytesAvailable() < (int) sizeof(int))
        {
            return;
        }

        in >> blockSize;
    }

    if (_socket->bytesAvailable() < blockSize)
    {
        return;
    }
    QByteArray blob;
    in >> blob;
    QDataStream in2(&blob, QIODevice::ReadOnly);
    in2.setVersion(QDataStream::Qt_5_3);

    process(in2);
    _socket->waitForBytesWritten();

    blob.clear();

    buildingBlob.clear();
    if (out != NULL)
    {
        delete out;
        out = NULL;
    }
}

void EDocTCPServerDatabasePlugin::error(QAbstractSocket::SocketError)
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::error(QAbstractSocket::SocketError)");
    logger->logError(_socket->errorString());
}

void EDocTCPServerDatabasePlugin::connected()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::connected()");
}

void EDocTCPServerDatabasePlugin::send()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::send()");
    blob.clear();
    QDataStream out(&blob, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (buildingBlob.size() + (int)sizeof(int));
    out << buildingBlob;

    _socket->write(blob);
    _socket->flush();
    _socket->waitForBytesWritten();
    blob.clear();
    blockSize = 0;
}

void EDocTCPServerDatabasePlugin::process(QDataStream &in)
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::process(QDataStream &in)");
    Header header;
    in >> header;

    MessageCodes::CodeNumber code = header.command();
    switch (code)
    {
    case MessageCodes::CodeNumber::REQ_fields:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_fields);
        QList<IFieldDefinition*> rsp = _persistance->fields();
        (*out) << rsp.count();
        foreach (IFieldDefinition* field, rsp)
        {
            (*out) << *field;
        }
        break;
    }
    case MessageCodes::CodeNumber::REQ_field:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_field);
        QString fieldName("");
        in >> fieldName;
        IFieldDefinition *fieldDef = _persistance->field(fieldName);
        (*out) << *fieldDef;
        break;
    }
    case MessageCodes::CodeNumber::REQ_createEmptyParameter:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_createEmptyParameter);
        IParameter *parameter = _persistance->createEmptyParameter();
        (*out) << *parameter;
        break;
    }
    case MessageCodes::CodeNumber::REQ_search:
    {
        int count = 0;
        in >> count;
        QList<IParameter*> parameters;
        for (int i = 0; i < count; ++i)
        {
            ProxyParameter *param = new ProxyParameter(this);
            in >> *param;
            parameters.push_back(param);
        }
        QList<IRecordID*> rsp = _persistance->search(parameters);
        prepareToSend(MessageCodes::CodeNumber::RSP_search);
        (*out) << rsp.count();
        foreach (IRecordID *record, rsp)
        {
            (*out) << *record;
        }
        break;
    }
    case MessageCodes::CodeNumber::REQ_createEnptyRecord:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_createEnptyRecord);
        IRecord *record = _persistance->createEmptyRecord();
        (*out) << *record;
        break;
    }
    case MessageCodes::CodeNumber::REQ_addRecord:
    {
        ProxyRecord record;
        in >> record;
        IRecordID *recordId = _persistance->addRecord(&record);
        prepareToSend(MessageCodes::CodeNumber::RSP_addRecord);
        (*out) << *recordId;
        break;
    }
    case MessageCodes::CodeNumber::REQ_getRecord:
    {
        ProxyRecordID *proxyRecordID = new ProxyRecordID(this);
        in >> *proxyRecordID;
        IRecord *record = _persistance->getRecord(proxyRecordID);
        prepareToSend(MessageCodes::CodeNumber::RSP_getRecord);
        (*out) << *record;
        break;
    }
    case MessageCodes::CodeNumber::REQ_getRecords:
    {
        QStringList ids;
        in >> ids;
        QList<IRecord*> records = _persistance->getRecords(ids);
        prepareToSend(MessageCodes::CodeNumber::RSP_getRecords);
        (*out) << records.count();
        foreach (IRecord* record, records)
        {
            (*out) << *record;
        }
        break;
    }
    case MessageCodes::CodeNumber::REQ_updateRecord:
        Q_ASSERT(false);
        break;
    case MessageCodes::CodeNumber::REQ_deleteRecord:
        Q_ASSERT(false);
        break;
    case MessageCodes::CodeNumber::REQ_getDistinctColumnValues:
    {
        QList<QPair<QString, QString>> filter;
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QPair<QString, QString> pair;
            in >> pair.first;
            in >> pair.second;
            filter.push_back(pair);
        }
        QString columnName;
        in >> columnName;
        QStringList rsp = _persistance->getDistinctColumnValues(filter, columnName);
        prepareToSend(MessageCodes::CodeNumber::RSP_getDistinctColumnValues);
        (*out) << rsp;
        break;
    }
    case MessageCodes::CodeNumber::REQ_addDocument:
    {
        QByteArray blob;
        in >> blob;
        IDocID *docId = _docEngine->addDocument(blob);
        prepareToSend(MessageCodes::CodeNumber::RSP_addDocument);
        (*out) << *docId;
        break;
    }
    default:
        Q_ASSERT(false);
        break;
    }
    send();
}

void EDocTCPServerDatabasePlugin::prepareToSend(MessageCodes::CodeNumber code)
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::prepareToSend(MessageCodes::CodeNumber code)");
    if (out != NULL)
    {
        delete out;
    }

    buildingBlob.clear();
    out = new QDataStream(&buildingBlob, QIODevice::WriteOnly);
    out->setVersion(QDataStream::Qt_5_3);

    Header header;
    header.setCommand(code);

    (*out) << header;
}
