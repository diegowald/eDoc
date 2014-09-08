#include "tcpserver.h"
#include "../eDoc-API/IFactory.h"
#include <QPluginLoader>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QApplication>

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    m_Name = "";
    port = 0;
    clientConnections.clear();
}

TcpServer::~TcpServer()
{
}

void TcpServer::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    logger = factory->logger();
    logger->logTrace(__FILE__, __LINE__, "EDocTCPServerDatabasePlugin", "void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();

    port = configuration.dynamicCast<XMLCollection>()->get("port").dynamicCast<XMLElement>()->value().toInt();
    XMLCollectionPtr confEngine = configuration.dynamicCast<XMLCollection>()->get("database").dynamicCast<XMLCollection>();
    dbh = factory->createDatabaseWithHistory(confEngine);
    if (!dbh.isNull())
    {
        database =  factory->createDatabase(confEngine);
    }

    confEngine = configuration.dynamicCast<XMLCollection>()->get("engine").dynamicCast<XMLCollection>();
    if (!confEngine.isNull())
    {
        docEngine = factory->createEngine(confEngine);
    }

    confEngine = configuration.dynamicCast<XMLCollection>()->get("tagengine").dynamicCast<XMLCollection>();
    if (!confEngine.isNull())
    {
        tagProcessor = factory->createTagProcessor(confEngine);
    }

    tcpServer = QSharedPointer<QTcpServer>(new QTcpServer());
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        logger->logError("Unable to start server");
    }

    connect(tcpServer.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void TcpServer::onNewConnection()
{
    logger->logDebug("void TcpServer::onNewConnection()");
    QSharedPointer<QTcpSocket> client = QSharedPointer<QTcpSocket>(tcpServer->nextPendingConnection());
    clientConnections[client] = QSharedPointer<EDocTCPServerDatabasePlugin>(new EDocTCPServerDatabasePlugin(logger, client, database, dbh, docEngine, tagProcessor, this));
    connect(clientConnections[client].data(), SIGNAL(finished()), this, SLOT(onClientConnectionFinished()));
}

void TcpServer::onClientConnectionFinished()
{
    QList<QSharedPointer<QTcpSocket>> deadSockets;
    foreach (QSharedPointer<QTcpSocket> socket, clientConnections.keys())
    {
        if (socket->state() == QAbstractSocket::UnconnectedState)
        {
            deadSockets.append(socket);
        }
    }
    foreach (QSharedPointer<QTcpSocket> socket, deadSockets)
    {
        clientConnections.remove(socket);
    }
}

void TcpServer::run()
{
    logger->logDebug("void TcpServer::run()");
    if (!tcpServer->isListening())
    {
        if (!tcpServer->listen(QHostAddress::Any, port))
        {
            logger->logError("Unable to start server");
        }
    }
    QApplication::processEvents();
    tcpServer->waitForNewConnection(1500);
}

void TcpServer::stop()
{
    logger->logDebug("void TcpServer::stop()");
    tcpServer->close();
}

QString TcpServer::name() const
{
    return "eDocTcpServerPlugin";
}

IServerPtr TcpServer::newServer()
{
    return IServerPtr(new TcpServer());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpServerPlugin, TcpServer)
#endif // QT_VERSION < 0x050000
