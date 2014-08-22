#include "tcpserver.h"
#include <QPluginLoader>

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    logger = NULL;
    m_Name = "";
    database = NULL;
    docEngine = NULL;
    tcpServer = NULL;
    port = 0;
    clientConnections.clear();
}

TcpServer::~TcpServer()
{
}

void TcpServer::initialize(IXMLContent *configuration, QObjectLogging *logger,
                           const QMap<QString, QString> &docpluginStock,
                           const QMap<QString, QString> &DBplugins,
                           const QMap<QString, QString> &tagPlugins,
                           const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    logger->logTrace(__FILE__, __LINE__, "EDocTCPServerDatabasePlugin", "void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    port = ((XMLElement*)((XMLCollection*) configuration)->get("port"))->value().toInt();
    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("database");
    database = createPersistentEngine(confEngine, docpluginStock, DBplugins, tagPlugins, serverPlugins);
    confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    docEngine = createDocEnginePersistance(confEngine, docpluginStock, DBplugins, tagPlugins, serverPlugins);

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        logger->logError("Unable to start server");
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

IDatabase *TcpServer::createPersistentEngine(XMLCollection *confEngine,
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

IDocEngine *TcpServer::createDocEnginePersistance(XMLCollection *confEngine,
                                       const QMap<QString, QString> &docpluginStock,
                                       const QMap<QString, QString> &DBplugins,
                                       const QMap<QString, QString> &tagPlugins,
                                       const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "engine")
    {
        XMLCollection *conf = (XMLCollection*) confEngine;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        if (docpluginStock.contains(engineClass)) {
            QPluginLoader pluginLoader(docpluginStock[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDocEngine* engine = qobject_cast<IDocEngine*>(plugin);
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
                return qobject_cast<IDocEngine *>(plugin);
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


void TcpServer::onNewConnection()
{
    logger->logDebug("void TcpServer::onNewConnection()");
    QTcpSocket *client = tcpServer->nextPendingConnection();
    clientConnections[client] = new EDocTCPServerDatabasePlugin(logger, client, database, docEngine, this);
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


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpServerPlugin, TcpServer)
#endif // QT_VERSION < 0x050000
