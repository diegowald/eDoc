#include "edoctcphistoricclient.h"
#include "../eDoc-API/IFactory.h"
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

void EDocTcpHistoricClient::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    logger = factory->logger();
    logger->logTrace(__FILE__, __LINE__, "EDocTcpHistoricClient", "void EDocTcpHistoricClient::initialize(IXMLContent *configuration, QObjectLogging *logger, \
                           const QMap<QString, QString> &docpluginStock, \
                           const QMap<QString, QString> &DBplugins, \
                           const QMap<QString, QString> &tagPlugins, \
                           const QMap<QString, QString> &serverPlugins)");
    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();

    ipAddress = configuration.dynamicCast<XMLCollection>()->get("url").dynamicCast<XMLElement>()->value();
    port = configuration.dynamicCast<XMLCollection>()->get("port").dynamicCast<XMLElement>()->value().toInt();
}

//IDatabase
QList<QSharedPointer<IFieldDefinition> > EDocTcpHistoricClient::fields()
{
    QList<QSharedPointer<IFieldDefinition>> lst;
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
            QSharedPointer<ProxyFieldDefinition> field = QSharedPointer<ProxyFieldDefinition>(new ProxyFieldDefinition());
            in >> *field;
            lst.push_back(field);
        }
    }
    return lst;
}

QSharedPointer<IFieldDefinition> EDocTcpHistoricClient::field(const QString &fieldName)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_field);
    (*out) << fieldName;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyFieldDefinition> field;
    if (header.command() == MessageCodes::CodeNumber::RSP_field)
    {
        field = QSharedPointer<ProxyFieldDefinition>(new ProxyFieldDefinition());
        in >> *field;
    }
    return field;
}

QSharedPointer<IParameter> EDocTcpHistoricClient::createEmptyParameter()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEmptyParameter);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyParameter> parameter = QSharedPointer<ProxyParameter>(new ProxyParameter());
    if (header.command() == MessageCodes::CodeNumber::RSP_createEmptyParameter)
    {
        in >> *parameter;
    }
    return parameter;
}

QList<QSharedPointer<IRecordID>> EDocTcpHistoricClient::search(const QList<QSharedPointer<IParameter> > &parameters)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_search);
    (*out) << parameters.count();
    foreach (QSharedPointer<IParameter> parameter, parameters)
    {
        (*out) << *parameter;
    }
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecordID> > lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_search)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecordID> record = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

QList<QSharedPointer<IRecordID> > EDocTcpHistoricClient::searchWithin(const QList<QSharedPointer<IParameter> > &parameters, const QList<QSharedPointer<IRecordID> > &records)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_searchWithin);
    (*out) << parameters.count();
    foreach (QSharedPointer<IParameter> parameter, parameters)
    {
        (*out) << *parameter;
    }
    (*out) << records.count();
    foreach (QSharedPointer<IRecordID> rec, records)
    {
        (*out) << *rec;
    }
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecordID> > lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_searchWithin)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecordID> record = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

QSharedPointer<IRecord> EDocTcpHistoricClient::createEmptyRecord()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEnptyRecord);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecord> record;
    if (header.command() == MessageCodes::CodeNumber::RSP_createEnptyRecord)
    {
        record = QSharedPointer<ProxyRecord>(new ProxyRecord());
        in >> *record;
    }
    return record;
}

QSharedPointer<IRecordID> EDocTcpHistoricClient::addRecord(QSharedPointer<IRecord> record)
{
    if (record.dynamicCast<ProxyRecord>())
    {
        record.dynamicCast<ProxyRecord>()->prepareToSave();
    }
    prepareToSend(MessageCodes::CodeNumber::REQ_addRecord);
    (*out) << *record;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecordID> recordId;
    if (header.command() == MessageCodes::CodeNumber::RSP_addRecord)
    {
        recordId = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
        in >> *recordId;
    }
    return recordId;
}

QSharedPointer<IRecord> EDocTcpHistoricClient::getRecord(QSharedPointer<IRecordID> id)
{
    return getRecord(id->asString());
}

QSharedPointer<IRecord> EDocTcpHistoricClient::getRecord(const QString &id)
{
    return getRecordByDate(id, QDateTime::currentDateTimeUtc());
}

QList<QSharedPointer<IRecord> > EDocTcpHistoricClient::getRecords(const QStringList &ids)
{
    return getRecordsByDate(ids, QDateTime::currentDateTimeUtc());
}

void EDocTcpHistoricClient::updateRecord(QSharedPointer<IRecord> record)
{
    if (record.dynamicCast<ProxyRecord>())
    {
        record.dynamicCast<ProxyRecord>()->prepareToSave();
    }
    prepareToSend(MessageCodes::CodeNumber::REQ_updateRecord);
    (*out) << *record;
    QByteArray response = send();
}

void EDocTcpHistoricClient::deleteRecord(QSharedPointer<IRecordID> id)
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

QList<QPair<QString, QString>> EDocTcpHistoricClient::getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
}

QString EDocTcpHistoricClient::name()
{
    return "EDocTcpHistoricClient";
}

QMap<QString, QSharedPointer<IRecordID> > EDocTcpHistoricClient::search(QSharedPointer<IParameter> parameter)
{
    QList<QSharedPointer<IParameter>> params;
    params.push_back(parameter);
    QList<QSharedPointer<IRecordID>> response = search(params);

    QMap<QString, QSharedPointer<IRecordID>> rsp;
    foreach (QSharedPointer<IRecordID> rec, response)
    {
        rsp[rec->asString()] = rec;
    }
    return rsp;
}

//IDatabaseWithHistory
QList<QSharedPointer<IRecordID>> EDocTcpHistoricClient::searchByDate(const QList<QSharedPointer<IParameter>> &parameters, const QDateTime &date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_searchWithHistory);
    (*out) << parameters.count();
    foreach (QSharedPointer<IParameter> parameter, parameters)
    {
        (*out) << *parameter;
    }
    (*out) << date;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecordID>> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_searchWithHistory)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecordID> record = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
            in >> *record;
            lst.push_back(record);
        }
    }
    return lst;
}

QSharedPointer<IRecord> EDocTcpHistoricClient::getRecordByDate(QSharedPointer<IRecordID> id, const QDateTime &date)
{
    return getRecordByDate(id->asString(), date);
}

QSharedPointer<IRecord> EDocTcpHistoricClient::getRecordByDate(const QString &id, const QDateTime &date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecordWithHistory);
    (*out) << id;
    (*out) << date;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecord> record;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecordWithHistory)
    {
        bool recordNull;
        in >> recordNull;
        if (!recordNull)
        {
            record = QSharedPointer<ProxyRecord>(new ProxyRecord());
            in >> *record;
        }
    }
    return record;
}

QList<QSharedPointer<IRecord>> EDocTcpHistoricClient::getRecordsByDate(const QStringList &ids, const QDateTime& date)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecordsWithHistory);
    (*out) << ids;
    (*out) << date;
    QByteArray response = send();
    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecord>> records;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecordsWithHistory)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecord> record = QSharedPointer<ProxyRecord>(new ProxyRecord());
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

QList<QSharedPointer<IRecord>> EDocTcpHistoricClient::getHistory(QSharedPointer<IRecordID> recordID)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getHistory);
    (*out) << recordID;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecord>> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_getHistory)
    {
        int count  = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecord> rec = QSharedPointer<ProxyRecord>(new ProxyRecord());
            in >> *rec;
            lst.append(rec);
        }
    }
    return lst;
}

QList<QSharedPointer<IRecordID>> EDocTcpHistoricClient::getChanges(const QDateTime &fromDate, const QDateTime &toDate)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getHistoryChanges);
    (*out) << fromDate;
    (*out) << toDate;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecordID>> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_getHistoryChanges)
    {
        int count  = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<ProxyRecordID> rec = QSharedPointer<ProxyRecordID>(new ProxyRecordID());
            in >> *rec;
            lst.append(rec);
        }
    }
    return lst;
}

QMap<QString, QSharedPointer<IRecordID>> EDocTcpHistoricClient::searchByDate(QSharedPointer<IParameter> parameter, const QDateTime &date)
{
    (void) parameter;
    (void) date;
    return QMap<QString, QSharedPointer<IRecordID>>();
}

IDatabasePtr EDocTcpHistoricClient::newDatabase()
{
    return IDatabasePtr(new EDocTcpHistoricClient());
}

IDatabaseWithHistoryPtr EDocTcpHistoricClient::newDatabaseWithHistory()
{
    return IDatabaseWithHistoryPtr(new EDocTcpHistoricClient());
}
