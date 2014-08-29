#include "edoctcpserverdatabaseplugin.h"
#include <QPluginLoader>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QDateTime>
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDocTCPMessages/messagecodes.h"
#include "../eDocTCPMessages/streamhelpers.h"

EDocTCPServerDatabasePlugin::EDocTCPServerDatabasePlugin(QSharedPointer<QObjectLogging> Logger,
                                                         QSharedPointer<QTcpSocket> socket,
                                                         QSharedPointer<IDatabase> persistance,
                                                         QSharedPointer<IDatabaseWithHistory> histPersistance,
                                                         QSharedPointer<IDocEngine> docEngine,
                                                         QObject *parent) :
    QThread(parent)
{
    _socket = socket;
    _persistance = persistance;
    _persistanceHist = histPersistance;
    _docEngine = docEngine;
    blockSize = 0;
    blob.clear();
    buildingBlob.clear();
    out = NULL;
    logger = Logger;

    connect(_socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(_socket.data(), SIGNAL(connected()), this, SLOT(connected()));
    start();

    functionMap[MessageCodes::CodeNumber::REQ_fields] = &EDocTCPServerDatabasePlugin::processREQFields;
    functionMap[MessageCodes::CodeNumber::REQ_field] = &EDocTCPServerDatabasePlugin::processREQField;
    functionMap[MessageCodes::CodeNumber::REQ_createEmptyParameter] = &EDocTCPServerDatabasePlugin::processREQcreateEmptyParameter;
    functionMap[MessageCodes::CodeNumber::REQ_search] = &EDocTCPServerDatabasePlugin::processREQSearch;
    functionMap[MessageCodes::CodeNumber::REQ_searchWithin] = &EDocTCPServerDatabasePlugin::processREQSearchWithin;
    functionMap[MessageCodes::CodeNumber::REQ_createEnptyRecord] = &EDocTCPServerDatabasePlugin::processREQCreateEnptyRecord;
    functionMap[MessageCodes::CodeNumber::REQ_addRecord] = &EDocTCPServerDatabasePlugin::processREQAddRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getRecord] = &EDocTCPServerDatabasePlugin::processREQGetRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getRecords] = &EDocTCPServerDatabasePlugin::processREQGetRecords;
    functionMap[MessageCodes::CodeNumber::REQ_updateRecord] = &EDocTCPServerDatabasePlugin::processREQUpdateRecord;
    functionMap[MessageCodes::CodeNumber::REQ_deleteRecord] = &EDocTCPServerDatabasePlugin::processREQDeleteRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getDistinctColumnValues] = &EDocTCPServerDatabasePlugin::processREQGetDistinctColumnValues;
    functionMap[MessageCodes::CodeNumber::REQ_addDocument] = &EDocTCPServerDatabasePlugin::processREQAddDocument;
    functionMap[MessageCodes::CodeNumber::REQ_searchWithHistory] = &EDocTCPServerDatabasePlugin::processREQSearchWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getRecordWithHistory] = &EDocTCPServerDatabasePlugin::processREQGetRecordWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getRecordsWithHistory] = &EDocTCPServerDatabasePlugin::processREQGetRecordsWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getDistinctColumnValuesWithHistory] = &EDocTCPServerDatabasePlugin::processREQGetDistinctColumnValuesWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getHistory] = &EDocTCPServerDatabasePlugin::processREQGetHistory;
    functionMap[MessageCodes::CodeNumber::RSP_getHistoryChanges] = &EDocTCPServerDatabasePlugin::processREQGetHistoryChanges;

}

EDocTCPServerDatabasePlugin::~EDocTCPServerDatabasePlugin()
{
    logger->logDebug("EDocTCPServerDatabasePlugin::~EDocTCPServerDatabasePlugin()");
}

void EDocTCPServerDatabasePlugin::run()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::run()");
    while (_socket->state() == QAbstractSocket::ConnectedState)
    {
        _socket->waitForReadyRead(1500);
        if (_socket->bytesAvailable() > 0)
        {
            onReadyRead();
        }
    }
    if ((_socket->state() == QAbstractSocket::ClosingState) || (_socket->state() == QAbstractSocket::UnconnectedState) || (_socket->state() == QAbstractSocket::ClosingState) || (_socket->state() == QAbstractSocket::UnconnectedState))
    {
        quit();
    }

}

void EDocTCPServerDatabasePlugin::onReadyRead()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::onReadyRead()");
    QDataStream in(_socket.data());
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
    QByteArray uncompressed = qUncompress(blob);
    //QDataStream in2(&blob, QIODevice::ReadOnly);
    QDataStream in2(&uncompressed, QIODevice::ReadOnly);
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

void EDocTCPServerDatabasePlugin::processREQFields(QDataStream &in)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_fields);
    QList<QSharedPointer<IFieldDefinition>> rsp = _persistanceHist ? _persistanceHist->fields() : _persistance->fields();
    (*out) << rsp.count();
    foreach (QSharedPointer<IFieldDefinition> field, rsp)
    {
        (*out) << *field;
    }
}

void EDocTCPServerDatabasePlugin::processREQField(QDataStream &in)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_field);
    QString fieldName("");
    in >> fieldName;
    QSharedPointer<IFieldDefinition> fieldDef = _persistanceHist ? _persistanceHist->field(fieldName) : _persistance->field(fieldName);
    (*out) << *fieldDef;
}

void EDocTCPServerDatabasePlugin::processREQcreateEmptyParameter(QDataStream &in)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_createEmptyParameter);
    QSharedPointer<IParameter> parameter = _persistanceHist ? _persistanceHist->createEmptyParameter() : _persistance->createEmptyParameter();
    (*out) << *parameter;
}

void EDocTCPServerDatabasePlugin::processREQSearch(QDataStream &in)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter>> parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter(this));
        in >> *param;
        parameters.push_back(param);
    }
    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist ? _persistanceHist->search(parameters) : _persistance->search(parameters);
    prepareToSend(MessageCodes::CodeNumber::RSP_search);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQSearchWithin(QDataStream &in)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter> > parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter(this));
        in >> *param;
        parameters.push_back(param);
    }

    in >> count;
    QList<QSharedPointer<IRecordID> > records;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyRecordID> rec = QSharedPointer<ProxyRecordID>(new ProxyRecordID(this));
        in >> *rec;
        records.push_back(rec);
    }

    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist ? _persistanceHist->searchWithin(parameters, records) : _persistance->searchWithin(parameters, records);
    prepareToSend(MessageCodes::CodeNumber::RSP_searchWithin);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQCreateEnptyRecord(QDataStream &in)
{
    (void) in;
    prepareToSend(MessageCodes::CodeNumber::RSP_createEnptyRecord);
    QSharedPointer<IRecord> record = _persistanceHist ? _persistanceHist->createEmptyRecord() : _persistance->createEmptyRecord();
    (*out) << *record;
}

void EDocTCPServerDatabasePlugin::processREQAddRecord(QDataStream &in)
{
    QSharedPointer<ProxyRecord> record = QSharedPointer<ProxyRecord>(new ProxyRecord());
    in >> *record;
    QSharedPointer<IRecordID> recordId = _persistanceHist ? _persistanceHist->addRecord(record) : _persistance->addRecord(record);
    prepareToSend(MessageCodes::CodeNumber::RSP_addRecord);
    (*out) << *recordId;
}

void EDocTCPServerDatabasePlugin::processREQGetRecord(QDataStream &in)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID(this));
    in >> *proxyRecordID;
    QSharedPointer<IRecord> record = _persistanceHist ? _persistanceHist->getRecord(proxyRecordID) : _persistance->getRecord(proxyRecordID);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecord);
    (*out) << *record;
}

void EDocTCPServerDatabasePlugin::processREQGetRecords(QDataStream &in)
{
    QStringList ids;
    in >> ids;
    QList<QSharedPointer<IRecord> > records = _persistanceHist ? _persistanceHist->getRecords(ids) : _persistance->getRecords(ids);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecords);
    (*out) << records.count();
    foreach (QSharedPointer<IRecord> record, records)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQUpdateRecord(QDataStream &in)
{
    Q_ASSERT(false);
}

void EDocTCPServerDatabasePlugin::processREQDeleteRecord(QDataStream &in)
{
    Q_ASSERT(false);
}

void EDocTCPServerDatabasePlugin::processREQGetDistinctColumnValues(QDataStream &in)
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
    QStringList rsp = _persistanceHist ?
                _persistanceHist->getDistinctColumnValues(filter, columnName) :
                _persistance->getDistinctColumnValues(filter, columnName);

    prepareToSend(MessageCodes::CodeNumber::RSP_getDistinctColumnValues);
    (*out) << rsp;
}

void EDocTCPServerDatabasePlugin::processREQAddDocument(QDataStream &in)
{
    QByteArray blob;
    in >> blob;
    QSharedPointer<IDocID> docId = _docEngine->addDocument(blob);
    prepareToSend(MessageCodes::CodeNumber::RSP_addDocument);
    (*out) << *docId;
}

void EDocTCPServerDatabasePlugin::processREQSearchWithHistory(QDataStream &in)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter> > parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter(this));
        in >> *param;
        parameters.push_back(param);
    }
    QDateTime date;
    in >> date;
    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist->searchByDate(parameters, date);
    prepareToSend(MessageCodes::CodeNumber::RSP_searchWithHistory);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQGetRecordWithHistory(QDataStream &in)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID(this));
    in >> *proxyRecordID;
    QDateTime date;
    in >> date;
    QSharedPointer<IRecord> record = _persistanceHist->getRecordByDate(proxyRecordID, date);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecordWithHistory);
    (*out) << *record;
}

void EDocTCPServerDatabasePlugin::processREQGetRecordsWithHistory(QDataStream &in)
{
    QStringList ids;
    in >> ids;
    QDateTime date;
    in >> date;
    QList<QSharedPointer<IRecord>> records = _persistanceHist->getRecordsByDate(ids, date);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecordsWithHistory);
    (*out) << records.count();
    foreach (QSharedPointer<IRecord> record, records)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQGetDistinctColumnValuesWithHistory(QDataStream &in)
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
    QDateTime date;
    in >> date;
    QStringList rsp = _persistanceHist->getDistinctColumnValuesByDate(filter, columnName, date);

    prepareToSend(MessageCodes::CodeNumber::RSP_getDistinctColumnValues);
    (*out) << rsp;
}

void EDocTCPServerDatabasePlugin::processREQGetHistory(QDataStream &in)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID(this));
    in >> *proxyRecordID;
    QDateTime date;
    in >> date;
    QList<QSharedPointer<IRecord> > records = _persistanceHist->getHistory(proxyRecordID);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecordWithHistory);
    (*out) << records.count();
    foreach (QSharedPointer<IRecord> record, records)
    {
        (*out) << *record;
    }
}

void EDocTCPServerDatabasePlugin::processREQGetHistoryChanges(QDataStream &in)
{
    QDateTime from;
    QDateTime to;
    in >> from >> to;
    QList<QSharedPointer<IRecordID> > lst = _persistanceHist->getChanges(from, to);
    prepareToSend(MessageCodes::CodeNumber::RSP_getHistoryChanges);
    (*out) << lst.count();
    foreach (QSharedPointer<IRecordID> rec, lst)
    {
        (*out) << *rec;
    }
}

void EDocTCPServerDatabasePlugin::send()
{
    logger->logDebug("void EDocTCPServerDatabasePlugin::send()");
    blob.clear();
    QDataStream out(&blob, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    QByteArray compressed = qCompress(buildingBlob, 9);
//    out << (buildingBlob.size() + (int)sizeof(int));
//    out << buildingBlob;
    out << (compressed.size() + (int)sizeof(int));
    out << compressed;

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
    if (functionMap.contains(code))
    {
        Executor functor = functionMap[code];
        (this->*functor)(in);
    }
    else
    {
        Q_ASSERT(false);
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
