#include "tcpserver.h"
#include <QPluginLoader>
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

void TcpServer::initialize(IXMLContent *configuration,
                           QSharedPointer<QObjectLogging> logger,
                           const QMap<QString, QString> &docpluginStock,
                           const QMap<QString, QString> &DBplugins,
                           const QMap<QString, QString> &DBWithHistoryPlugins,
                           const QMap<QString, QString> &tagPlugins,
                           const QMap<QString, QString> &serverPlugins)
{
    this->logger = logger;
    logger->logTrace(__FILE__, __LINE__, "EDocTCPServerDatabasePlugin", "void EDocTCPServerDatabasePlugin::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    port = ((XMLElement*)((XMLCollection*) configuration)->get("port"))->value().toInt();
    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("database");
    dbh = createHistoryDBPersistentEngine(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
    if (!dbh)
    {
        database = createPersistentEngine(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
    }
    confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    docEngine = createDocEnginePersistance(confEngine, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);

    tcpServer = QSharedPointer<QTcpServer>(new QTcpServer(this));
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        logger->logError("Unable to start server");
    }

    connect(tcpServer.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

QSharedPointer<IDatabase> TcpServer::createPersistentEngine(XMLCollection *confEngine,
                                             const QMap<QString, QString> &docpluginStock,
                                             const QMap<QString, QString> &DBplugins,
                                             const QMap<QString, QString> &DBWithHistoryPlugins,
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
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return QSharedPointer<IDatabase>(qobject_cast<IDatabase *>(plugin));
            }
            else {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return QSharedPointer<IDatabase>();
}

QSharedPointer<IDatabaseWithHistory> TcpServer::createHistoryDBPersistentEngine(XMLCollection *confEngine,
                                                      const QMap<QString, QString> &docpluginStock,
                                                      const QMap<QString, QString> &DBplugins,
                                                      const QMap<QString, QString> &DBWithHistoryPlugins,
                                                      const QMap<QString, QString> &tagPlugins,
                                                      const QMap<QString, QString> &serverPlugins)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "database")
    {
        XMLCollection *conf = (XMLCollection*) confEngine;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        if (DBWithHistoryPlugins.contains(engineClass)) {
            QPluginLoader pluginLoader(DBWithHistoryPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDatabaseWithHistory* engine = qobject_cast<IDatabaseWithHistory*>(plugin);
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return QSharedPointer<IDatabaseWithHistory>(engine);
            }
            else {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return QSharedPointer<IDatabaseWithHistory>();
}

QSharedPointer<IDocEngine> TcpServer::createDocEnginePersistance(XMLCollection *confEngine,
                                       const QMap<QString, QString> &docpluginStock,
                                       const QMap<QString, QString> &DBplugins, const QMap<QString, QString> &DBWithHistoryPlugins,
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
                engine->initialize(confEngine, logger, docpluginStock, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return QSharedPointer<IDocEngine>(engine);
            }
            else {
                logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return QSharedPointer<IDocEngine>();
}


void TcpServer::onNewConnection()
{
    logger->logDebug("void TcpServer::onNewConnection()");
    QSharedPointer<QTcpSocket> client = QSharedPointer<QTcpSocket>(tcpServer->nextPendingConnection());
    clientConnections[client] = QSharedPointer<EDocTCPServerDatabasePlugin>(new EDocTCPServerDatabasePlugin(logger, client, database, dbh, docEngine, this));
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


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDocTcpServerPlugin, TcpServer)
#endif // QT_VERSION < 0x050000
