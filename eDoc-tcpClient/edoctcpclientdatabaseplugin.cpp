
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

void eDocTcpClientDatabasePlugin::initialize(IXMLContent *configuration,
                                             QSharedPointer<QObjectLogging> logger,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins,
                                             const QMap<QString, QString> &DBWithHistoryPlugins,
                                             const QMap<QString, QString> &tagPlugins,
                                             const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    this->logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    ipAddress = ((XMLElement*)((XMLCollection*) configuration)->get("url"))->value();
    port = ((XMLElement*)((XMLCollection*) configuration)->get("port"))->value().toInt();
}



QList<QSharedPointer<IFieldDefinition>> eDocTcpClientDatabasePlugin::fields()
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

QSharedPointer<IFieldDefinition> eDocTcpClientDatabasePlugin::field(const QString &fieldName)
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
        field = QSharedPointer<ProxyFieldDefinition>(new ProxyFieldDefinition(this));
        in >> *field;
    }
    return field;
}

QSharedPointer<IParameter> eDocTcpClientDatabasePlugin::createEmptyParameter()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEmptyParameter);
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    QSharedPointer<IParameter> parameter = QSharedPointer<IParameter>(new ProxyParameter(this));
    if (header.command() == MessageCodes::CodeNumber::RSP_createEmptyParameter)
    {
        in >> *parameter.dynamicCast<ProxyParameter>();
    }
    return parameter;
}

QList<QSharedPointer<IRecordID> > eDocTcpClientDatabasePlugin::search(const QList<QSharedPointer<IParameter>> &parameters)
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
            QSharedPointer<IRecordID> record = QSharedPointer<IRecordID>(new ProxyRecordID(this));
            in >> *record.dynamicCast<ProxyRecordID>();
            lst.push_back(record);
        }
    }
    return lst;
}

QList<QSharedPointer<IRecordID>> eDocTcpClientDatabasePlugin::searchWithin(const QList<QSharedPointer<IParameter>> &parameters, const QList<QSharedPointer<IRecordID>> &records)
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
            QSharedPointer<IRecordID> record = QSharedPointer<IRecordID>(new ProxyRecordID(this));
            in >> *record.dynamicCast<ProxyRecord>();
            lst.push_back(record);
        }
    }
    return lst;
}

QSharedPointer<IRecord> eDocTcpClientDatabasePlugin::createEmptyRecord()
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
        record = QSharedPointer<ProxyRecord>(new ProxyRecord(this));
        in >> *record;
    }
    return record;
}

QSharedPointer<IRecordID> eDocTcpClientDatabasePlugin::addRecord(QSharedPointer<IRecord> record)
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
        recordId = QSharedPointer<ProxyRecordID>(new ProxyRecordID(this));
        in >> *recordId;
    }
    return recordId;
}

QSharedPointer<IRecord> eDocTcpClientDatabasePlugin::getRecord(QSharedPointer<IRecordID> id)
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
        record = QSharedPointer<ProxyRecord>(new ProxyRecord(this));
        in >> *record;
    }
    return record;
}

QSharedPointer<IRecord> eDocTcpClientDatabasePlugin::getRecord(const QString &id)
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
        record = QSharedPointer<ProxyRecord>(new ProxyRecord(this));
        in >> *record;
    }
    return record;
}

QList<QSharedPointer<IRecord> > eDocTcpClientDatabasePlugin::getRecords(const QStringList &ids)
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
            QSharedPointer<ProxyRecord> record = QSharedPointer<ProxyRecord>(new ProxyRecord(this));
            in >> *record;
            records.append(record);
        }
    }
    return records;
}

void eDocTcpClientDatabasePlugin::updateRecord(QSharedPointer<IRecord> record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_updateRecord);
    (*out) << *record;
    QByteArray response = send();
}

void eDocTcpClientDatabasePlugin::deleteRecord(QSharedPointer<IRecordID> id)
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

QMap<QString, QSharedPointer<IRecordID>> eDocTcpClientDatabasePlugin::search(QSharedPointer<IParameter> parameter)
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
QSharedPointer<IDocID> eDocTcpClientDatabasePlugin::addDocument(const QByteArray& blob)
{
    QSharedPointer<ProxyDocID> docId;
    prepareToSend(MessageCodes::CodeNumber::REQ_addDocument);
    (*out) << blob;
    QByteArray response = send();

    QDataStream in(&response, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);

    Header header;
    in >> header;
    if (header.command() == MessageCodes::CodeNumber::RSP_addDocument)
    {
        docId = QSharedPointer<ProxyDocID>(new ProxyDocID(this));
        in >> *docId;
    }
    return docId;
}

QSharedPointer<IDocBase> eDocTcpClientDatabasePlugin::getDocument(QSharedPointer<IDocID> id)
{
}

bool eDocTcpClientDatabasePlugin::deleteDocument(QSharedPointer<IDocID> id)
{
}

QSharedPointer<IDocID> eDocTcpClientDatabasePlugin::IValueToIDocId(QSharedPointer<IValue> value)
{
}

// ITagProcessor
void eDocTcpClientDatabasePlugin::addTagRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag)
{
}

QSet<QString> eDocTcpClientDatabasePlugin::findByTags(const QStringList &tags)
{
}

void eDocTcpClientDatabasePlugin::removeRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag)
{
}

void eDocTcpClientDatabasePlugin::processKeywordString(QSharedPointer<IRecordID> recordID, const QString &keywords)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpClient, eDocTcpClientDatabasePlugin)
#endif // QT_VERSION < 0x050000

