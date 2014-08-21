#include "edoctcpclientdatabaseplugin.h"

#include <QtPlugin>
#include <QtNetwork/QNetworkConfigurationManager>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDocTCPMessages/streamhelpers.h"


eDocTcpClientDatabasePlugin::eDocTcpClientDatabasePlugin(QObject *parent) :QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    //networkSession = NULL;
    ipAddress = "";
    port = 0;
    out = NULL;
    timeOut = 15000;

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
}

eDocTcpClientDatabasePlugin::~eDocTcpClientDatabasePlugin()
{
    if (tcpSocket->isOpen())
    {
        tcpSocket->close();
    }
    if (out != NULL)
    {
        delete out;
    }
}

void eDocTcpClientDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins,
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

IRecord* eDocTcpClientDatabasePlugin::createEmptyRecord()
{
    prepareToSend(MessageCodes::CodeNumber::REQ_createEnptyRecord);
    QByteArray response = send();
}

IRecordID *eDocTcpClientDatabasePlugin::addRecord(IRecord *record)
{
    prepareToSend(MessageCodes::CodeNumber::REQ_addRecord);
    (*out) << *record;
    QByteArray response = send();
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

QByteArray eDocTcpClientDatabasePlugin::send()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(ipAddress, port);
    if (!tcpSocket->waitForConnected(timeOut))
    {
        //error
        return NULL;
    }
    blob.clear();
    QDataStream out(&blob, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    out << (buildingBlob.size() + (int)sizeof(int));
    out << buildingBlob;

    tcpSocket->write(blob);
    tcpSocket->flush();
    tcpSocket->waitForBytesWritten();
    blob.clear();

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_3);


    while (tcpSocket->bytesAvailable() < (int) sizeof(int))
    {
        if (!tcpSocket->waitForReadyRead(timeOut))
        {
            // error
        }
    }
    in >> blockSize;

    while (tcpSocket->bytesAvailable() < blockSize)
    {
        if (!tcpSocket->waitForReadyRead(timeOut) < blockSize)
        {
            // error
        }
    }

    QByteArray response;
    in >> response;
    return response;
}

//void eDocTcpClientDatabasePlugin::send(const QByteArray &blobToSend)
//{
//    tcpSocket->abort();
//    tcpSocket->connectToHost(ipAddress, port);
//
//    blob = blobToSend;
//}

void eDocTcpClientDatabasePlugin::readyRead()
{
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_5_3);
//
//    if (blockSize == 0)
//    {
//        if (tcpSocket->bytesAvailable() < (int) sizeof(int))
//        {
//            return;
//        }
//
//        in >> blockSize;
//    }
//
//    if (tcpSocket->bytesAvailable() < blockSize)
//    {
//        return;
//    }
//
//    Header header;
//    in >> header;
//
//    processHeader(header);
}

void eDocTcpClientDatabasePlugin::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
        logger->logError(tr("The host was not found. Please check the "
                                     "host name and port settings."));
         break;
     case QAbstractSocket::ConnectionRefusedError:
        logger->logError(tr("The connection was refused by the peer. "
                                     "Make sure the fortune server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
         break;
     default:
        logger->logError(tr("The following error occurred: %1.")
                         .arg(tcpSocket->errorString()));
     }
}

void eDocTcpClientDatabasePlugin::connected()
{
//    tcpSocket->write(blob);
//    tcpSocket->flush();
//    tcpSocket->waitForBytesWritten();
//    blob.clear();
}

void eDocTcpClientDatabasePlugin::prepareToSend(MessageCodes::CodeNumber code)
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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpClient, eDocTcpClientDatabasePlugin)
#endif // QT_VERSION < 0x050000

