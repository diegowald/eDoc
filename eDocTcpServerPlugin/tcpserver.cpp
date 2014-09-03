#include "tcpserver.h"
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

void TcpServer::initialize(IXMLContentPtr configuration,
                           QObjectLoggingPtr logger,
                           const QMap<QString, QString> &docpluginStock,
                           const QMap<QString, QString> &DBplugins,
                           const QMap<QString, QString> &DBWithHistoryPlugins,
                           const QMap<QString, QString> &tagPlugins,
                           const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    logger->logTrace(__FILE__, __LINE__, "EDocTCPServerDatabasePlugin", "void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();

    port = configuration.dynamicCast<XMLCollection>()->get("port").dynamicCast<XMLElement>()->value().toInt();
    XMLCollectionPtr confEngine = configuration.dynamicCast<XMLCollection>()->get("database").dynamicCast<XMLCollection>();
    dbh = createHistoryDBPersistentEngine(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
    if (!dbh)
    {
        database = createPersistentEngine(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
    }

    confEngine = configuration.dynamicCast<XMLCollection>()->get("engine").dynamicCast<XMLCollection>();
    docEngine = createDocEnginePersistance(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);

    confEngine = configuration.dynamicCast<XMLCollection>()->get("tagengine").dynamicCast<XMLCollection>();
    tagProcessor = createTagProcessor(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);

    tcpServer = QSharedPointer<QTcpServer>(new QTcpServer());
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        logger->logError("Unable to start server");
    }

    connect(tcpServer.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

IDatabasePtr TcpServer::createPersistentEngine(XMLCollectionPtr confEngine,
                                               const QMap<QString, QString> &docpluginStock,
                                               const QMap<QString, QString> &DBplugins,
                                               const QMap<QString, QString> &DBWithHistoryPlugins,
                                               const QMap<QString, QString> &tagPlugins,
                                               const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "database")
    {
        //XMLCollection *conf = (XMLCollection*) confEngine;
        XMLCollectionPtr conf = confEngine.dynamicCast<XMLCollection>();
        QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

        if (DBplugins.contains(engineClass))
        {
            QPluginLoader pluginLoader(DBplugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IDatabase* engineCreator = qobject_cast<IDatabase*>(plugin);
                IDatabasePtr engine = engineCreator->newDatabase();
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return IDatabasePtr(qobject_cast<IDatabase *>(plugin));
            }
            else
            {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else
        {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return IDatabasePtr();
}

IDatabaseWithHistoryPtr TcpServer::createHistoryDBPersistentEngine(XMLCollectionPtr confEngine,
                                                                   const QMap<QString, QString> &docpluginStock,
                                                                   const QMap<QString, QString> &DBplugins,
                                                                   const QMap<QString, QString> &DBWithHistoryPlugins,
                                                                   const QMap<QString, QString> &tagPlugins,
                                                                   const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "database")
    {
        XMLCollectionPtr conf = confEngine.dynamicCast<XMLCollection>();
        QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

        if (DBWithHistoryPlugins.contains(engineClass))
        {
            QPluginLoader pluginLoader(DBWithHistoryPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IDatabaseWithHistory* engineCreator = qobject_cast<IDatabaseWithHistory*>(plugin);
                IDatabaseWithHistoryPtr engine = engineCreator->newDatabaseWithHistory();
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return IDatabaseWithHistoryPtr(engine);
            }
            else
            {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else
        {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return IDatabaseWithHistoryPtr();
}

IDocEnginePtr TcpServer::createDocEnginePersistance(XMLCollectionPtr confEngine,
                                                    const QMap<QString, QString> &docpluginStock,
                                                    const QMap<QString, QString> &DBplugins,
                                                    const QMap<QString, QString> &DBWithHistoryPlugins,
                                                    const QMap<QString, QString> &tagPlugins,
                                                    const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "engine")
    {
        XMLCollectionPtr conf = confEngine.dynamicCast<XMLCollection>();
        QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

        if (docpluginStock.contains(engineClass))
        {
            QPluginLoader pluginLoader(docpluginStock[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IDocEngine* engineCreator = qobject_cast<IDocEngine*>(plugin);
                IDocEnginePtr engine = engineCreator->newDocEngine();
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return IDocEnginePtr(engine);
            }
            else
            {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else
        {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return IDocEnginePtr();
}

ITagProcessorPtr TcpServer::createTagProcessor(XMLCollectionPtr confEngine,
                                    const QMap<QString, QString> &docpluginStock,
                                    const QMap<QString, QString> &DBplugins,
                                    const QMap<QString, QString> &DBWithHistoryPlugins,
                                    const QMap<QString, QString> &tagPlugins,
                                    const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "tagengine")
    {
        XMLCollectionPtr conf = confEngine.dynamicCast<XMLCollection>();
        QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

        if (tagPlugins.contains(engineClass))
        {
            QPluginLoader pluginLoader(tagPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                ITagProcessor *engineCreator = qobject_cast<ITagProcessor*>(plugin);
                ITagProcessorPtr engine = engineCreator->newTagProcessor();
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return engine;
            }
            else
            {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else
        {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return ITagProcessorPtr();
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
