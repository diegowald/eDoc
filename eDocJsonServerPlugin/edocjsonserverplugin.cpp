#include "edocjsonserverplugin.h"
#include <QPluginLoader>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDocTCPMessages/messagecodes.h"
#include "../eDocTCPMessages/streamhelpers.h"

#include "jsonhelpers.h"

EDocJsonServerPlugin::EDocJsonServerPlugin(QObjectLoggingPtr Logger,
                                                         QSharedPointer<QTcpSocket> socket,
                                                         IDatabasePtr persistance,
                                                         IDatabaseWithHistoryPtr histPersistance,
                                                         IDocEnginePtr docEngine,
                                                         ITagProcessorPtr tagProcessor,
                                                         QObject *parent) :
    QThread(parent)
{
    _socket = socket;
    _persistance = persistance;
    _persistanceHist = histPersistance;
    _docEngine = docEngine;
    _tagProcessor = tagProcessor;
    blockSize = 0;
    logger = Logger;

    connect(_socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(_socket.data(), SIGNAL(connected()), this, SLOT(connected()));
    start();

    functionMap[MessageCodes::CodeNumber::REQ_fields] = &EDocJsonServerPlugin::processREQFields;
    functionMap[MessageCodes::CodeNumber::REQ_field] = &EDocJsonServerPlugin::processREQField;
    functionMap[MessageCodes::CodeNumber::REQ_createEmptyParameter] = &EDocJsonServerPlugin::processREQcreateEmptyParameter;
    /*functionMap[MessageCodes::CodeNumber::REQ_search] = &EDocJsonServerPlugin::processREQSearch;
    functionMap[MessageCodes::CodeNumber::REQ_searchWithin] = &EDocJsonServerPlugin::processREQSearchWithin;
    functionMap[MessageCodes::CodeNumber::REQ_createEnptyRecord] = &EDocJsonServerPlugin::processREQCreateEnptyRecord;
    functionMap[MessageCodes::CodeNumber::REQ_addRecord] = &EDocJsonServerPlugin::processREQAddRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getRecord] = &EDocJsonServerPlugin::processREQGetRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getRecords] = &EDocJsonServerPlugin::processREQGetRecords;
    functionMap[MessageCodes::CodeNumber::REQ_updateRecord] = &EDocJsonServerPlugin::processREQUpdateRecord;
    functionMap[MessageCodes::CodeNumber::REQ_deleteRecord] = &EDocJsonServerPlugin::processREQDeleteRecord;
    functionMap[MessageCodes::CodeNumber::REQ_getDistinctColumnValues] = &EDocJsonServerPlugin::processREQGetDistinctColumnValues;
    functionMap[MessageCodes::CodeNumber::REQ_addDocument] = &EDocJsonServerPlugin::processREQcreateDocument;
    functionMap[MessageCodes::CodeNumber::REQ_addDocumentWithOriginalLocation] = &EDocJsonServerPlugin::processREQcreateDocumentWithName;
    functionMap[MessageCodes::CodeNumber::REQ_searchWithHistory] = &EDocJsonServerPlugin::processREQSearchWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getRecordWithHistory] = &EDocJsonServerPlugin::processREQGetRecordWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getRecordsWithHistory] = &EDocJsonServerPlugin::processREQGetRecordsWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getDistinctColumnValuesWithHistory] = &EDocJsonServerPlugin::processREQGetDistinctColumnValuesWithHistory;
    functionMap[MessageCodes::CodeNumber::REQ_getHistory] = &EDocJsonServerPlugin::processREQGetHistory;
    functionMap[MessageCodes::CodeNumber::RSP_getHistoryChanges] = &EDocJsonServerPlugin::processREQGetHistoryChanges;
    functionMap[MessageCodes::CodeNumber::REQ_addTagRecord] = &EDocJsonServerPlugin::processREQAddTagRecord;
    functionMap[MessageCodes::CodeNumber::REQ_findByTags] = &EDocJsonServerPlugin::processREQFindByTags;
    functionMap[MessageCodes::CodeNumber::REQ_removeRecord] = &EDocJsonServerPlugin::processREQRemoveRecord;
    functionMap[MessageCodes::CodeNumber::REQ_processKeywordString] = &EDocJsonServerPlugin::processREQprocessKeywordString;
    functionMap[MessageCodes::CodeNumber::REQ_processKeywordStringList] = &EDocJsonServerPlugin::processREQprocessKeywordStringList;*/
}

EDocJsonServerPlugin::~EDocJsonServerPlugin()
{
    logger->logDebug("EDocJsonServerPlugin::~EDocJsonServerPlugin()");
}

void EDocJsonServerPlugin::run()
{
    logger->logDebug("void EDocJsonServerPlugin::run()");
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

void EDocJsonServerPlugin::onReadyRead()
{
    logger->logDebug("void EDocJsonServerPlugin::onReadyRead()");
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
    QDataStream in2(&uncompressed, QIODevice::ReadOnly);
    in2.setVersion(QDataStream::Qt_5_3);

    process(in2);
    _socket->waitForBytesWritten();

    blob.clear();
}

void EDocJsonServerPlugin::error(QAbstractSocket::SocketError)
{
    logger->logDebug("void EDocJsonServerPlugin::error(QAbstractSocket::SocketError)");
    logger->logError(_socket->errorString());
}

void EDocJsonServerPlugin::connected()
{
    logger->logDebug("void EDocJsonServerPlugin::connected()");
}

QJsonObject EDocJsonServerPlugin::processREQFields(QJsonObject &input)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_fields);
    QList<QSharedPointer<IFieldDefinition>> rsp = _persistanceHist ? _persistanceHist->fields() : _persistance->fields();

    QJsonObject ressponse;
    ressponse["count"] = rsp.count();
    QJsonArray fields;
    foreach (QSharedPointer<IFieldDefinition> field, rsp)
    {
        fields.append(JsonHelpers::toJson(field));
    }
    response["fields"] = fields;
    return response;
}

QJsonObject EDocJsonServerPlugin::processREQField(QJsonObject &input)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_field);
    QString fieldName("");
    fieldName = input["fieldName"].toString();
    QSharedPointer<IFieldDefinition> fieldDef = _persistanceHist ? _persistanceHist->field(fieldName) : _persistance->field(fieldName);
    return JsonHelpers::toJson(fieldDef);
}

QJsonObject EDocJsonServerPlugin::processREQcreateEmptyParameter(QJsonObject &input)
{
    prepareToSend(MessageCodes::CodeNumber::RSP_createEmptyParameter);
    QSharedPointer<IParameter> parameter = _persistanceHist ? _persistanceHist->createEmptyParameter() : _persistance->createEmptyParameter();
    return JsonHelpers::toJson(parameter);
}

/*QJsonObject EDocJsonServerPlugin::processREQSearch(QJsonObject &input)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter>> parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter());
        in >> param;
        parameters.push_back(param);
    }
    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist ? _persistanceHist->search(parameters) : _persistance->search(parameters);
    prepareToSend(MessageCodes::CodeNumber::RSP_search);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQSearchWithin(QJsonObject &input)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter> > parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter());
        in >> param;
        parameters.push_back(param);
    }

    in >> count;
    QList<QSharedPointer<IRecordID> > records;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyRecordID> rec = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
        in >> rec;
        records.push_back(rec);
    }

    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist ? _persistanceHist->searchWithin(parameters, records) : _persistance->searchWithin(parameters, records);
    prepareToSend(MessageCodes::CodeNumber::RSP_searchWithin);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQCreateEnptyRecord(QJsonObject &input)
{
    (void) in;
    prepareToSend(MessageCodes::CodeNumber::RSP_createEnptyRecord);
    QSharedPointer<IRecord> record = _persistanceHist ? _persistanceHist->createEmptyRecord() : _persistance->createEmptyRecord();
    (*out) << record;
}

QJsonObject EDocJsonServerPlugin::processREQAddRecord(QJsonObject &input)
{
    QSharedPointer<ProxyRecord> record = QSharedPointer<ProxyRecord>(new ProxyRecord());
    in >> record;
    QSharedPointer<IRecordID> recordId = _persistanceHist ? _persistanceHist->addRecord(record) : _persistance->addRecord(record);
    prepareToSend(MessageCodes::CodeNumber::RSP_addRecord);
    (*out) << recordId;
}

QJsonObject EDocJsonServerPlugin::processREQGetRecord(QJsonObject &input)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
    in >> proxyRecordID;
    QSharedPointer<IRecord> record = _persistanceHist ? _persistanceHist->getRecord(proxyRecordID) : _persistance->getRecord(proxyRecordID);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecord);
    (*out) << record;
}

QJsonObject EDocJsonServerPlugin::processREQGetRecords(QJsonObject &input)
{
    QStringList ids;
    in >> ids;
    QList<QSharedPointer<IRecord> > records = _persistanceHist ? _persistanceHist->getRecords(ids) : _persistance->getRecords(ids);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecords);
    (*out) << records.count();
    foreach (QSharedPointer<IRecord> record, records)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQUpdateRecord(QJsonObject &input)
{
    Q_ASSERT(false);

    QSharedPointer<IRecord> record = QSharedPointer<IRecord>(new ProxyRecord());
    in >> record.dynamicCast<ProxyRecord>();

    if (_persistanceHist.isNull())
    {
        _persistance->updateRecord(record);
    }
    else
    {
        _persistanceHist->updateRecord(record);
    }
}

QJsonObject EDocJsonServerPlugin::processREQDeleteRecord(QJsonObject &input)
{
    Q_ASSERT(false);
}

QJsonObject EDocJsonServerPlugin::processREQGetDistinctColumnValues(QJsonObject &input)
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

QJsonObject EDocJsonServerPlugin::processREQcreateDocument(QJsonObject &input)
{
    QByteArray blob;
    in >> blob;
    IDocBasePtr doc = _docEngine->createDocument(blob);
    prepareToSend(MessageCodes::CodeNumber::RSP_addDocument);
    (*out) << doc->id();
}

QJsonObject EDocJsonServerPlugin::processREQcreateDocumentWithName(QJsonObject &input)
{
    QString originalLocation;
    QByteArray blob;
    IDocBasePtr doc = _docEngine->createDocument(originalLocation, blob);
    prepareToSend(MessageCodes::CodeNumber::RSP_addDocumentWithOriginalLocation);
    (*out) << doc->id();
}

QJsonObject EDocJsonServerPlugin::processREQSearchWithHistory(QJsonObject &input)
{
    int count = 0;
    in >> count;
    QList<QSharedPointer<IParameter> > parameters;
    for (int i = 0; i < count; ++i)
    {
        QSharedPointer<ProxyParameter> param = QSharedPointer<ProxyParameter>(new ProxyParameter());
        in >> param;
        parameters.push_back(param);
    }
    QDateTime date;
    in >> date;
    QList<QSharedPointer<IRecordID>> rsp = _persistanceHist->searchByDate(parameters, date);
    prepareToSend(MessageCodes::CodeNumber::RSP_searchWithHistory);
    (*out) << rsp.count();
    foreach (QSharedPointer<IRecordID> record, rsp)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQGetRecordWithHistory(QJsonObject &input)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
    in >> proxyRecordID;
    QDateTime date;
    in >> date;
    QSharedPointer<IRecord> record = _persistanceHist->getRecordByDate(proxyRecordID, date);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecordWithHistory);
    bool recordNull = record.isNull();
    (*out) << recordNull;
    if (!recordNull)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQGetRecordsWithHistory(QJsonObject &input)
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
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQGetDistinctColumnValuesWithHistory(QJsonObject &input)
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

QJsonObject EDocJsonServerPlugin::processREQGetHistory(QJsonObject &input)
{
    QSharedPointer<ProxyRecordID> proxyRecordID = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
    in >> proxyRecordID;
    QDateTime date;
    in >> date;
    QList<QSharedPointer<IRecord> > records = _persistanceHist->getHistory(proxyRecordID);
    prepareToSend(MessageCodes::CodeNumber::RSP_getRecordWithHistory);
    (*out) << records.count();
    foreach (QSharedPointer<IRecord> record, records)
    {
        (*out) << record;
    }
}

QJsonObject EDocJsonServerPlugin::processREQGetHistoryChanges(QJsonObject &input)
{
    QDateTime from;
    QDateTime to;
    in >> from >> to;
    QList<QSharedPointer<IRecordID> > lst = _persistanceHist->getChanges(from, to);
    prepareToSend(MessageCodes::CodeNumber::RSP_getHistoryChanges);
    (*out) << lst.count();
    foreach (QSharedPointer<IRecordID> rec, lst)
    {
        (*out) << rec;
    }
}

QJsonObject EDocJsonServerPlugin::processREQAddTagRecord(QJsonObject &in)
{
    Q_ASSERT(false);
}

QJsonObject EDocJsonServerPlugin::processREQFindByTags(QJsonObject &in)
{
    int count = 0;
    in >> count;
    QStringList tags;
    QString tag;
    for (int i = 0; i < count; ++i)
    {
        in >> tag;
        tags.append(tag);
    }

    QSet<QString> result = _tagProcessor->findByTags(tags);

    prepareToSend(MessageCodes::CodeNumber::RSP_findByTags);

    (*out) << result.count();
    foreach (QString res, result.toList())
    {
        (*out) << res;
    }
}

QJsonObject EDocJsonServerPlugin::processREQRemoveRecord(QJsonObject &input)
{
    Q_ASSERT(false);
}

QJsonObject EDocJsonServerPlugin::processREQprocessKeywordString(QJsonObject &input)
{
    ProxyRecordIDPtr proxyRecordID = ProxyRecordIDPtr(new ProxyRecordID());
    in >> proxyRecordID;
    QString keywords;
    in >> keywords;
    _tagProcessor->processKeywordString(proxyRecordID, keywords);
    prepareToSend(MessageCodes::CodeNumber::RSP_processKeywordString);
    (*out) << true;
}

QJsonObject EDocJsonServerPlugin::processREQprocessKeywordStringList(QJsonObject &input)
{
    ProxyRecordIDPtr proxyRecordID = ProxyRecordIDPtr(new ProxyRecordID());
    in >> proxyRecordID;
    QStringList keywords;
    in >> keywords;
    _tagProcessor->processKeywordStringList(proxyRecordID, keywords);
    prepareToSend(MessageCodes::CodeNumber::RSP_processKeywordStringList);
    (*out) << true;
}*/

void EDocJsonServerPlugin::send()
{
    logger->logDebug("void EDocJsonServerPlugin::send()");

    QByteArray blob;
    QDataStream out(&blob, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    QJsonDocument doc(response);
    out << doc.toJson();

    _socket->write(blob);
    _socket->flush();
    _socket->waitForBytesWritten();
    blob.clear();
    blockSize = 0;
}

void EDocJsonServerPlugin::process(QDataStream &in)
{
    logger->logDebug("void EDocJsonServerPlugin::process(QDataStream &in)");
    QByteArray input;
    in >> input;

    QJsonDocument jsonInput = QJsonDocument::fromJson(input);
    QJsonObject jsonInputObject = jsonInput.object();

    MessageCodes::CodeNumber code = MessageCodes::fromString(jsonInputObject["command"].toString());
    if (functionMap.contains(code))
    {
        QJsonObject response;
        QJsonObject parameters = jsonInputObject["parameters"].toObject();
        Executor functor = functionMap[code];
        response = (this->*functor)(parameters);
    }
    else
    {
        Q_ASSERT(false);
    }

    send();
}

void EDocJsonServerPlugin::prepareToSend(MessageCodes::CodeNumber code)
{
    logger->logDebug("void EDocJsonServerPlugin::prepareToSend(MessageCodes::CodeNumber code)");
    response = QJsonObject();
    response["command"] = MessageCodes::toString(code);
}
