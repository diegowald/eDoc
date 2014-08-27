
#include "edoctcpclientdatabaseplugin.h"

#include <QtPlugin>
#include <QtNetwork/QNetworkConfigurationManager>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/streamhelpers.h"

eDocTcpClientDatabasePlugin::eDocTcpClientDatabasePlugin(QObject *parent) :TcpClient(parent)
{
}

eDocTcpClientDatabasePlugin::~eDocTcpClientDatabasePlugin()
{
}

void eDocTcpClientDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins, const QMap<QString, QString> &DBWithHistoryPlugins,
                                             const QMap<QString, QString> &tagPlugins,
                                             const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    this->logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    ipAddress = ((XMLElement*)((XMLCollection*) configuration)->get("url"))->value();
    port = ((XMLElement*)((XMLCollection*) configuration)->get("port"))->value().toInt();
}



QList<IFieldDefinition*> eDocTcpClientDatabasePlugin::fields()
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

IFieldDefinition* eDocTcpClientDatabasePlugin::field(const QString &fieldName)
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

IParameter* eDocTcpClientDatabasePlugin::createEmptyParameter()
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

QList<IRecordID*> eDocTcpClientDatabasePlugin::search(const QList<IParameter*> &parameters)
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

QList<IRecordID*> eDocTcpClientDatabasePlugin::searchWithin(const QList<IParameter *> &parameters, const QList<IRecordID *> &records)
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

IRecord* eDocTcpClientDatabasePlugin::createEmptyRecord()
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

IRecordID *eDocTcpClientDatabasePlugin::addRecord(IRecord *record)
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

IRecord* eDocTcpClientDatabasePlugin::getRecord(IRecordID *id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecord);
    (*out) << *id;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyRecord *record = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecord)
    {
        record = new ProxyRecord(this);
        in >> *record;
    }
    return record;
}

IRecord* eDocTcpClientDatabasePlugin::getRecord(const QString &id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecord);
    (*out) << id;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyRecord *record = NULL;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecord)
    {
        record = new ProxyRecord(this);
        in >> *record;
    }
    return record;
}

QList<IRecord*> eDocTcpClientDatabasePlugin::getRecords(const QStringList &ids)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecords);
    (*out) << ids;
    QByteArray response = send();
    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<IRecord*> records;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecords)
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

void eDocTcpClientDatabasePlugin::updateRecord(IRecord* record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_updateRecord);
    (*out) << *record;
    QByteArray response = send();
}

void eDocTcpClientDatabasePlugin::deleteRecord(IRecordID *id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_deleteRecord);
    (*out) << *id;
    QByteArray response = send();
}

QStringList eDocTcpClientDatabasePlugin::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
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

QString eDocTcpClientDatabasePlugin::name()
{
    return "EDocTcpClient";
}

QMap<QString, IRecordID*> eDocTcpClientDatabasePlugin::search(IParameter* parameter)
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

// IDocEngine
IDocID* eDocTcpClientDatabasePlugin::addDocument(const QByteArray& blob)
{
    ProxyDocID *docId = NULL;
    prepareToSend(MessageCodes::CodeNumber::REQ_addDocument);
    (*out) << blob;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_addDocument)
    {
        docId = new ProxyDocID(this);
        in >> *docId;
    }
    return docId;
}

IDocBase* eDocTcpClientDatabasePlugin::getDocument(IDocID *id)
{
}

bool eDocTcpClientDatabasePlugin::deleteDocument(IDocID *id)
{
}

IDocID* eDocTcpClientDatabasePlugin::IValueToIDocId(IValue* value)
{
}

// ITagProcessor
void eDocTcpClientDatabasePlugin::addTagRecord(IRecordID *recordID, ITag* tag)
{
}

QSet<QString> eDocTcpClientDatabasePlugin::findByTags(const QStringList &tags)
{
}

void eDocTcpClientDatabasePlugin::removeRecord(IRecordID* recordID, ITag* tag)
{
}

void eDocTcpClientDatabasePlugin::processKeywordString(IRecordID *recordID, const QString &keywords)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpClient, eDocTcpClientDatabasePlugin)
#endif // QT_VERSION < 0x050000

