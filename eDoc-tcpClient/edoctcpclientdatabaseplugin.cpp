#include "edoctcpclientdatabaseplugin.h"

#include <QtPlugin>
#include <QtNetwork/QNetworkConfigurationManager>
#include "../eDoc-API/IFactory.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/streamhelpers.h"
#include "../eDocTCPMessages/proxydocument.h"


eDocTcpClientDatabasePlugin::eDocTcpClientDatabasePlugin(QObject *parent) :TcpClient(parent)
{
}

eDocTcpClientDatabasePlugin::~eDocTcpClientDatabasePlugin()
{
}

void eDocTcpClientDatabasePlugin::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    logger = factory->logger();
    logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");

    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();
    ipAddress = configuration.dynamicCast<XMLCollection>()->get("url").dynamicCast<XMLElement>()->value();
    port = configuration.dynamicCast<XMLCollection>()->get("port").dynamicCast<XMLElement>()->value().toInt();
}

QList<IFieldDefinitionPtr> eDocTcpClientDatabasePlugin::fields()
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
            QSharedPointer<IFieldDefinition> field = QSharedPointer<IFieldDefinition>(new ProxyFieldDefinition(this));
            in >> *field.dynamicCast<ProxyFieldDefinition>();
            lst.push_back(field);
        }
    }
    return lst;
}

IFieldDefinitionPtr eDocTcpClientDatabasePlugin::field(const QString &fieldName)
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

IParameterPtr eDocTcpClientDatabasePlugin::createEmptyParameter()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEmptyParameter);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<IParameter> parameter = QSharedPointer<IParameter>(new ProxyParameter());
    if (header.command() == MessageCodes::CodeNumber::RSP_createEmptyParameter)
    {
        in >> *parameter.dynamicCast<ProxyParameter>();
    }
    return parameter;
}

QList<IRecordIDPtr> eDocTcpClientDatabasePlugin::search(const QList<IParameterPtr> &parameters)
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
    QList<QSharedPointer<IRecordID>> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_search)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<IRecordID> record = QSharedPointer<IRecordID>(new ProxyRecordID());
            in >> *record.dynamicCast<ProxyRecordID>();
            lst.push_back(record);
        }
    }
    return lst;
}

QList<IRecordIDPtr> eDocTcpClientDatabasePlugin::searchWithin(const QList<IParameterPtr> &parameters, const QList<IRecordIDPtr> &records)
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
    QList<QSharedPointer<IRecordID>> lst;
    if (header.command() == MessageCodes::CodeNumber::RSP_searchWithin)
    {
        int count = 0;
        in >> count;
        for (int i = 0; i < count; ++i)
        {
            QSharedPointer<IRecordID> record = QSharedPointer<IRecordID>(new ProxyRecordID());
            in >> *record.dynamicCast<ProxyRecord>();
            lst.push_back(record);
        }
    }
    return lst;
}

IRecordPtr eDocTcpClientDatabasePlugin::createEmptyRecord()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEnptyRecord);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecord>record;
    if (header.command() == MessageCodes::CodeNumber::RSP_createEnptyRecord)
    {
        record = QSharedPointer<ProxyRecord>(new ProxyRecord());
        in >> *record;
    }
    return record;
}

IRecordIDPtr eDocTcpClientDatabasePlugin::addRecord(IRecordPtr record)
{
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

IRecordPtr eDocTcpClientDatabasePlugin::getRecord(IRecordIDPtr id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecord);
    (*out) << *id;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecord> record;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecord)
    {
        record = QSharedPointer<ProxyRecord>(new ProxyRecord());
        in >> *record;
    }
    return record;
}

IRecordPtr eDocTcpClientDatabasePlugin::getRecord(const QString &id)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecord);
    (*out) << id;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<ProxyRecord> record;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecord)
    {
        record = QSharedPointer<ProxyRecord>(new ProxyRecord());
        in >> *record;
    }
    return record;
}

QList<IRecordPtr> eDocTcpClientDatabasePlugin::getRecords(const QStringList &ids)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_getRecords);
    (*out) << ids;
    QByteArray response = send();
    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QList<QSharedPointer<IRecord>> records;
    if (header.command() == MessageCodes::CodeNumber::RSP_getRecords)
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

void eDocTcpClientDatabasePlugin::updateRecord(IRecordPtr record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_updateRecord);
    (*out) << *record;
    QByteArray response = send();
}

void eDocTcpClientDatabasePlugin::deleteRecord(IRecordIDPtr id)
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

QList<QPair<QString, QString>> eDocTcpClientDatabasePlugin::getColumnValue(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
}

QString eDocTcpClientDatabasePlugin::name()
{
    return "EDocTcpClient";
}

QMap<QString, IRecordIDPtr> eDocTcpClientDatabasePlugin::search(IParameterPtr parameter)
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

// IDocEngine

IDocBasePtr eDocTcpClientDatabasePlugin::createDocument(const QString sourcePath, const QByteArray &blob)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_addDocumentWithOriginalLocation);
    (*out) << sourcePath;
    (*out) << blob;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    ProxyDocIDPtr docId;
    if (header.command() == MessageCodes::CodeNumber::RSP_addDocumentWithOriginalLocation)
    {
        docId = ProxyDocIDPtr(new ProxyDocID());
        in >> *docId;
    }
    return IDocBasePtr(new ProxyDocument(docId));
}

IDocBasePtr eDocTcpClientDatabasePlugin::createDocument(const QByteArray& blob)
{
    ProxyDocIDPtr docId;
    prepareToSend(MessageCodes::CodeNumber::REQ_addDocument);
    (*out) << blob;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_addDocument)
    {
        docId = ProxyDocIDPtr(new ProxyDocID());
        in >> *docId;
    }
    return IDocBasePtr(new ProxyDocument(docId));
}

IDocBasePtr eDocTcpClientDatabasePlugin::getDocument(IDocIDPtr id)
{
}

IDocBasePtr eDocTcpClientDatabasePlugin::getDocument(const QString &id)
{
}

bool eDocTcpClientDatabasePlugin::deleteDocument(IDocIDPtr id)
{
}

IDocIDPtr eDocTcpClientDatabasePlugin::IValueToIDocId(IValuePtr value)
{
}

// ITagProcessor
void eDocTcpClientDatabasePlugin::addTagRecord(IRecordIDPtr recordID, ITagPtr tag)
{
}

QSet<QString> eDocTcpClientDatabasePlugin::findByTags(const QStringList &tags)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_findByTags);
    (*out) << tags.count();
    foreach (QString tag, tags)
    {
        (*out) << tag;
    }

    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSet<QString> responseSet;
    if (header.command() == MessageCodes::CodeNumber::RSP_findByTags)
    {
        int count = 0;
        in >> count;
        QString tag;
        for (int i = 0; i < count; ++i)
        {
            in >> tag;
            responseSet.insert(tag);
        }
    }
    return responseSet;
}

void eDocTcpClientDatabasePlugin::removeRecord(IRecordIDPtr recordID, ITagPtr tag)
{
}

void eDocTcpClientDatabasePlugin::processKeywordString(IRecordIDPtr recordID, const QString &keywords)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_processKeywordString);
    (*out) << *recordID;
    (*out) << keywords;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_processKeywordString)
    {
        bool ok = false;
        in >> ok;
    }
}

void eDocTcpClientDatabasePlugin::processKeywordStringList(IRecordIDPtr recordID, const QStringList &keywords)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_processKeywordStringList);
    (*out) << *recordID;
    (*out) << keywords;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_processKeywordStringList)
    {
        bool ok = false;
        in >> ok;
    }
}

IDatabasePtr eDocTcpClientDatabasePlugin::newDatabase()
{
    return IDatabasePtr(new eDocTcpClientDatabasePlugin());
}

IDocEnginePtr eDocTcpClientDatabasePlugin::newDocEngine()
{
    return IDocEnginePtr(new eDocTcpClientDatabasePlugin());
}

ITagProcessorPtr eDocTcpClientDatabasePlugin::newTagProcessor()
{
    return ITagProcessorPtr(new eDocTcpClientDatabasePlugin());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpClient, eDocTcpClientDatabasePlugin)
#endif // QT_VERSION < 0x050000

