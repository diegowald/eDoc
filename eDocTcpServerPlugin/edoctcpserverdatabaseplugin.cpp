#include "edoctcpserverdatabaseplugin.h"
#include <QPluginLoader>
#include <QtNetwork/QTcpSocket>

#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDocTCPMessages/messagecodes.h"
#include "../eDocTCPMessages/streamhelpers.h"

EDocTCPServerDatabasePlugin::EDocTCPServerDatabasePlugin(QTcpSocket *socket, IDatabase *persistance, QObject *parent) :
    QObject(parent)
{
    _persistance = NULL;
    _socket = socket;
    _persistance = persistance;
    blockSize = 0;
    blob.clear();
    buildingBlob.clear();
    out = NULL;

    connect(_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(_socket, SIGNAL(connected()), this, SLOT(connected()));

    _socket->waitForReadyRead();
}

EDocTCPServerDatabasePlugin::~EDocTCPServerDatabasePlugin()
{
}

void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins,
                                             const QMap<QString, QString> &tagPlugins,
                                             const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    this->logger->logTrace(__FILE__, __LINE__, "EDocTCPServerDatabasePlugin", "void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("database");
    _persistance = createPersistentEngine(confEngine, docpluginStock, DBplugins, tagPlugins, serverPlugins);
}

QList<IFieldDefinition*> EDocTCPServerDatabasePlugin::fields()
{
    return _persistance->fields();
}

IFieldDefinition* EDocTCPServerDatabasePlugin::field(const QString &fieldName)
{
    return _persistance->field(fieldName);
}

IParameter* EDocTCPServerDatabasePlugin::createEmptyParameter()
{
    return _persistance->createEmptyParameter();
}

QList<IRecordID*> EDocTCPServerDatabasePlugin::search(const QList<IParameter*> &parameters)
{
    return _persistance->search(parameters);
}

IRecord* EDocTCPServerDatabasePlugin::createEmptyRecord()
{
    return _persistance->createEmptyRecord();
}

IRecordID *EDocTCPServerDatabasePlugin::addRecord(IRecord *record)
{
    return _persistance->addRecord(record);
}

IRecord* EDocTCPServerDatabasePlugin::getRecord(IRecordID *id)
{
    return _persistance->getRecord(id);
}

IRecord* EDocTCPServerDatabasePlugin::getRecord(const QString &id)
{
    return _persistance->getRecord(id);
}

void EDocTCPServerDatabasePlugin::updateRecord(IRecord* record)
{
    _persistance->updateRecord(record);
}

void EDocTCPServerDatabasePlugin::deleteRecord(IRecordID *id)
{
    _persistance->deleteRecord(id);
}

QStringList EDocTCPServerDatabasePlugin::getDistinctColumnValues(const QList<QPair<QString, QString> >& filter, const QString & columnName)
{
    return _persistance->getDistinctColumnValues(filter, columnName);
}

QString EDocTCPServerDatabasePlugin::name()
{
    return "EDocTCPServerDatabasePlugin";
}

QMap<QString, IRecordID*> EDocTCPServerDatabasePlugin::search(IParameter* parameter)
{
    return QMap<QString, IRecordID*>();
}

void EDocTCPServerDatabasePlugin::onReadyRead()
{
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
}

void EDocTCPServerDatabasePlugin::error(QAbstractSocket::SocketError)
{
}

void EDocTCPServerDatabasePlugin::connected()
{
}

void EDocTCPServerDatabasePlugin::send()
{
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
    Header header;
    in >> header;

    MessageCodes::CodeNumber code = header.command();
    switch (code)
    {
    case MessageCodes::CodeNumber::REQ_fields:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_fields);
        QList<IFieldDefinition*> rsp = fields();
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
        IFieldDefinition *fieldDef = field(fieldName);
        (*out) << *fieldDef;
        break;
    }
    case MessageCodes::CodeNumber::REQ_createEmptyParameter:
    {
        prepareToSend(MessageCodes::CodeNumber::RSP_createEmptyParameter);
        IParameter *parameter = createEmptyParameter();
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
        QList<IRecordID*> rsp = search(parameters);
        prepareToSend(MessageCodes::CodeNumber::RSP_search);
        (*out) << rsp.count();
        foreach (IRecordID *record, rsp)
        {
            (*out) << *record;
        }
        break;
    }
    case MessageCodes::CodeNumber::REQ_createEnptyRecord:
        break;
    case MessageCodes::CodeNumber::REQ_addRecord:
        break;
    case MessageCodes::CodeNumber::REQ_getRecord:
    {
        ProxyRecordID *proxyRecordID = new ProxyRecordID(this);
        in >> *proxyRecordID;
        IRecord *record = getRecord(proxyRecordID);
        prepareToSend(MessageCodes::CodeNumber::RSP_getRecord);
        (*out) << *record;
        break;
    }
    case MessageCodes::CodeNumber::REQ_updateRecord:
        break;
    case MessageCodes::CodeNumber::REQ_deleteRecord:
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
        QStringList rsp = getDistinctColumnValues(filter, columnName);
        prepareToSend(MessageCodes::CodeNumber::RSP_getDistinctColumnValues);
        (*out) << rsp;
        break;
    }
    default:
        break;
    }
    send();
}

void EDocTCPServerDatabasePlugin::prepareToSend(MessageCodes::CodeNumber code)
{
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

IDatabase *EDocTCPServerDatabasePlugin::createPersistentEngine(XMLCollection *confEngine,
                                                               const QMap<QString, QString> &docpluginStock,
                                                               const QMap<QString, QString> &DBplugins,
                                                               const QMap<QString, QString> &tagPlugins,
                                                               const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "database")
    {
        XMLCollection *conf = (XMLCollection*) confEngine;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        if (DBplugins.contains(engineClass)) {
            QPluginLoader pluginLoader(DBplugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDatabase* engine = qobject_cast<IDatabase*>(plugin);
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
                return qobject_cast<IDatabase *>(plugin);
            }
            else {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return NULL;
}
