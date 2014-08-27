#include "edoctcphistoricclient.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/streamhelpers.h"
#include <QDateTime>

EDocTcpHistoricClient::EDocTcpHistoricClient(QObject *parent) : TcpClient(parent)
{
}

EDocTcpHistoricClient::~EDocTcpHistoricClient()
{
}

void EDocTcpHistoricClient::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                       const QMap<QString, QString> &docpluginStock,
                                       const QMap<QString, QString> &DBplugins,
                                       const QMap<QString, QString> &DBWithHistoryPlugins,
                                       const QMap<QString, QString> &tagPlugins,
                                       const QMap<QString, QString> &serverPlugins)
{
    (void)docpluginStock;
    (void)DBplugins;
    (void)DBWithHistoryPlugins;
    (void)tagPlugins;
    (void)serverPlugins;

    this->logger = logger;
    this->logger->logTrace(__FILE__, __LINE__, "EDocTcpHistoricClient", "void EDocTcpHistoricClient::initialize(IXMLContent *configuration, QObjectLogging *logger, \
                           const QMap<QString, QString> &docpluginStock, \
                           const QMap<QString, QString> &DBplugins, \
                           const QMap<QString, QString> &tagPlugins, \
                           const QMap<QString, QString> &serverPlugins)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    ipAddress = ((XMLElement*)((XMLCollection*) configuration)->get("url"))->value();
    port = ((XMLElement*)((XMLCollection*) configuration)->get("port"))->value().toInt();
}

//IDatabase
QList<IFieldDefinition*> EDocTcpHistoricClient::fields()
{
    QList<IFieldDefinition*> lst;
    prepareToSend(MessageCodes::CodeNumber::REQ_fields);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_fields)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyFieldDefinition *field = new ProxyFieldDefinition(this);
            in >> *field;
            lst.push_back(field);
        }
    }
    return lst;
}

IFieldDefinition* EDocTcpHistoricClient::field(const QString &fieldName)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_field);
    (*out) << fieldName;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyFieldDefinition *field = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_field)
    {
        field = new ProxyFieldDefinition(this);
        in >> *field;
    }
    return field;
}

IParameter* EDocTcpHistoricClient::createEmptyParameter()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEmptyParameter);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyParameter *parameter = new ProxyParameter(this);
    if (header.command() == MessageCodes::CodeNumber::RSP_createEmptyParameter)
    {
        in >> *parameter;
    }
    return parameter;
}

QList<IRecordID*> EDocTcpHistoricClient::search(const QList<IParameter*> &parameters)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_search);
    (*out) << parameters.count();
    foreach (IParameter *parameter, parameters)
    {
        (*out) << *parameter;
    }
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecordID*> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_search)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecordID* record = new ProxyRecordID(this);
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

QList<IRecordID*> EDocTcpHistoricClient::searchWithin(const QList<IParameter *> &parameters, const QList<IRecordID *> &records)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_searchWithin);
    (*out) << parameters.count();
    foreach (IParameter *parameter, parameters)
    {
        (*out) << *parameter;
    }
    (*out) << records.count();
    foreach (IRecordID* rec, records)
    {
        (*out) << *rec;
    }
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecordID*> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_searchWithin)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecordID* record = new ProxyRecordID(this);
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

IRecord* EDocTcpHistoricClient::createEmptyRecord()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEnptyRecord);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyRecord *record  = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_createEnptyRecord)
    {
        record = new ProxyRecord(this);
        in >> *record;
    }
    return record;
}

IRecordID *EDocTcpHistoricClient::addRecord(IRecord *record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_addRecord);
    (*out) << *record;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyRecordID *recordId  = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_addRecord)
    {
        recordId = new ProxyRecordID(this);
        in >> *recordId;
    }
    return recordId;
}

IRecord* EDocTcpHistoricClient::getRecord(IRecordID *id)
{
    return getRecord(id->asString());
}

IRecord* EDocTcpHistoricClient::getRecord(const QString &id)
{
    return getRecordByDate(id, QDateTime::currentDateTimeUtc());
}

QList<IRecord*> EDocTcpHistoricClient::getRecords(const QStringList &ids)
{
    return getRecordsByDate(ids, QDateTime::currentDateTimeUtc());
}

void EDocTcpHistoricClient::updateRecord(IRecord* record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_updateRecord);
    (*out) << *record;
    QByteArray response = send();
}

void EDocTcpHistoricClient::deleteRecord(IRecordID *id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_deleteRecord);
    (*out) << *id;
    QByteArray response = send();
}

QStringList EDocTcpHistoricClient::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    QStringList rsp;
    prepareToSend(MessageCodes::CodeNumber::REQ_getDistinctColumnValues);
    (*out) << filter.count();
    foreach (auto filterElement, filter)
    {
        (*out) << filterElement.first;
        (*out) << filterElement.second;
    }
    (*out) << columnName;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_getDistinctColumnValues)
    {
        in >> rsp;
    }
    return rsp;
}

QString EDocTcpHistoricClient::name()
{
    return "EDocTcpHistoricClient";
}

QMap<QString, IRecordID*> EDocTcpHistoricClient::search(IParameter* parameter)
{
    QList<IParameter*> params;
    params.push_back(parameter);
    QList<IRecordID*> response = search(params);

    QMap<QString, IRecordID*> rsp;
    foreach (IRecordID* rec, response)
    {
        rsp[rec->asString()] = rec;
    }
    return rsp;
}

//IDatabaseWithHistory
QList<IRecordID*> EDocTcpHistoricClient::searchByDate(const QList<IParameter*> &parameters, const QDateTime &date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_searchWithHistory);
    (*out) << parameters.count();
    foreach (IParameter *parameter, parameters)
    {
        (*out) << *parameter;
    }
    (*out) << date;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecordID*> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_searchWithHistory)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecordID* record = new ProxyRecordID(this);
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

IRecord* EDocTcpHistoricClient::getRecordByDate(IRecordID *id, const QDateTime &date)
{
    return getRecordByDate(id->asString(), date);
}

IRecord* EDocTcpHistoricClient::getRecordByDate(const QString &id, const QDateTime &date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecordWithHistory);
    (*out) << id;
    (*out) << date;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyRecord *record = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecordWithHistory)
    {
        record = new ProxyRecord(this);
        in >> *record;
    }
    return record;
}

QList<IRecord*> EDocTcpHistoricClient::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecordsWithHistory);
    (*out) << ids;
    (*out) << date;
    QByteArray response = send();
    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecord*> records;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecordsWithHistory)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecord *record = new ProxyRecord(this);
            in >> *record;
            records.append(record);
        }
    }
    return records;
}

QStringList EDocTcpHistoricClient::getDistinctColumnValuesByDate(const QList<QPair<QString, QString> >& filter, const QString & columnName, const QDateTime &date)
{
    QStringList rsp;
    prepareToSend(MessageCodes::CodeNumber::REQ_getDistinctColumnValuesWithHistory);
    (*out) << filter.count();
    foreach (auto filterElement, filter)
    {
        (*out) << filterElement.first;
        (*out) << filterElement.second;
    }
    (*out) << columnName;
    (*out) << date;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_getDistinctColumnValuesWithHistory)
    {
        in >> rsp;
    }
    return rsp;
}

QList<IRecord*> EDocTcpHistoricClient::getHistory(IRecordID *recordID)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getHistory);
    (*out) << recordID;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecord*> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_getHistory)
    {
        int count  = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecord *rec = new ProxyRecord(this);
            in >> *rec;
            lst.append(rec);
        }
    }
    return lst;
}

QList<IRecordID*> EDocTcpHistoricClient::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getHistoryChanges);
    (*out) << fromDate;
    (*out) << toDate;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecordID*> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_getHistoryChanges)
    {
        int count  = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            ProxyRecordID *rec = new ProxyRecordID(this);
            in >> *rec;
            lst.append(rec);
        }
    }
    return lst;
}

QMap<QString, IRecordID*> EDocTcpHistoricClient::searchByDate(IParameter* parameter, const QDateTime &date)
{
    (void) parameter;
    (void) date;
    return QMap<QString, IRecordID*>();
}
