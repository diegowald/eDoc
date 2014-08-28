#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>
#include "configreader.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "queryengine.h"
#include "../eDoc-API/IDocument.h"
#include "databasewithhistorywrapper.h"

EDocFactory::EDocFactory() :
    pluginPath(""), xmlFile(""),
    plugins(), DBWithHistoryPlugins(), DBplugins(), tagPlugins(), serverPlugins(),
    configuration(NULL), engine(NULL), database(NULL), query(NULL), tagger(NULL),
    m_Logger(NULL), server(NULL)
{
}

EDocFactory::~EDocFactory()
{
}

void EDocFactory::readAvailablePlugins()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "void EDocFactory::readAvailablePlugins()");
    QDir pluginsDir(pluginPath);
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QString f = pluginsDir.absoluteFilePath(fileName);
        m_Logger->logDebug("File: " + f);
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin == NULL)
        {
            m_Logger->logError(pluginLoader.errorString());
        }
        if (plugin) {
            IDocEngine* engine = qobject_cast<IDocEngine *>(plugin);
            if (engine)
            {
                plugins[engine->name()] = f;
                m_Logger->logDebug("Engine Name: " + engine->name() + ", File: " + f);
            }
            IDatabase *db = qobject_cast<IDatabase *>(plugin);
            if (db)
            {
                DBplugins[db->name()] = f;
                m_Logger->logDebug("DBEngine Name: " + db->name() + ", File: " + f);
            }
            IDatabaseWithHistory *dbh = qobject_cast<IDatabaseWithHistory *>(plugin);
            if (dbh)
            {
                DBWithHistoryPlugins[dbh->name()] = f;
                m_Logger->logDebug("DBWithHistoryEngine Name: " + dbh->name() + ", File: " + f);
            }
            ITagProcessor *tag = qobject_cast<ITagProcessor*>(plugin);
            if (tag)
            {
                tagPlugins[tag->name()] = f;
                m_Logger->logDebug("Tag Processor Name: " + tag->name() + ", File: " + f);
            }
            IServer *serv = qobject_cast<IServer *>(plugin);
            if (serv)
            {
                serverPlugins[serv->name()] = f;
                m_Logger->logDebug("Tag Processor Name: " + serv->name() + ", File: " + f);
            }
        }
        delete plugin;
    }
}

void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile, QSharedPointer<QObjectLogging> logger)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile)");
    this->xmlFile = xmlFile;
    this->pluginPath = pluginPath;
    readAvailablePlugins();

    // Ahora a leer e instanciar y configurar el plugin
    ConfigReader reader(this->xmlFile);
    configuration = reader.getConfiguration();
    m_Logger->logDebug(configuration->toDebugString());
    engine = createEngine();
    database = createDatabase();
    query = createQueryEngine();
    tagger = createTagEngine();
    server = createServerEngine();
}

QSharedPointer<IDocEngine> EDocFactory::docEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine* EDocFactory::docEngine()");
    return engine;
}

QSharedPointer<IDatabaseWithHistory> EDocFactory::databaseEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase* EDocFactory::databaseEngine()");
    return database;
}

QSharedPointer<IQueryEngine> EDocFactory::queryEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IQueryEngine * EDocFactory::queryEngine()");
    return query;
}

QSharedPointer<ITagProcessor> EDocFactory::tagEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "ITagProcessor *EDocFactory::tagEngine()");
    return tagger;
}

QSharedPointer<IServer> EDocFactory::serverEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IServer *EDocFactory::serverEngine()");
    return server;
}

QSharedPointer<IDocEngine> EDocFactory::createEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine *EDocFactory::createEngine()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("engine");
        if (conf)
        {
            QString engineClass = ((XMLElement*)conf->get("class"))->value();
            QPluginLoader pluginLoader(plugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDocEngine * engine = qobject_cast<IDocEngine*>(plugin);
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);

                return QSharedPointer<IDocEngine>(qobject_cast<IDocEngine *>(plugin));
            }
            m_Logger->logError("Cannot create Engine " + engineClass);
        }
    }
    m_Logger->logError("Cannot create Engine");
    return QSharedPointer<IDocEngine>();
}

QSharedPointer<IDatabaseWithHistory> EDocFactory::createDatabase()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase *EDocFactory::createDatabase()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("database");
        if (conf)
        {
            QString engineClass = ((XMLElement*)conf->get("class"))->value();
            if (DBWithHistoryPlugins.contains(engineClass))
            {
                QPluginLoader pluginLoader(DBWithHistoryPlugins[engineClass]);
                QObject *plugin = pluginLoader.instance();
                if (plugin) {
                    IDatabaseWithHistory *engine = qobject_cast<IDatabaseWithHistory*>(plugin);
                    engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                    return QSharedPointer<IDatabaseWithHistory>(qobject_cast<IDatabaseWithHistory *>(plugin));
                }
            }
            else
            {
                return createDatabaseWithoutHistory();
            }
        }
    }
    m_Logger->logError("Cannot create database engine");
    return QSharedPointer<IDatabaseWithHistory>();
}

QSharedPointer<IDatabaseWithHistory> EDocFactory::createDatabaseWithoutHistory()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabaseWithHistory *EDocFactory::createDatabaseWithoutHistory()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("database");
        if (conf)
        {
            QString engineClass = ((XMLElement*)conf->get("class"))->value();
            QPluginLoader pluginLoader(DBplugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                QSharedPointer<IDatabase> engine = QSharedPointer<IDatabase>(qobject_cast<IDatabase*>(plugin));
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                QSharedPointer<IDatabaseWithHistory> db = QSharedPointer<IDatabaseWithHistory>(new DatabaseWithHistoryWrapper(engine, m_Logger));
                return db;
            }
        }
    }
    m_Logger->logError("Cannot create database engine");
    return QSharedPointer<IDatabaseWithHistory>();
}

QSharedPointer<IQueryEngine> EDocFactory::createQueryEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IQueryEngine *EDocFactory::createQueryEngine()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("queries");
        if (conf)
        {
            query = QSharedPointer<QueryEngine>(new QueryEngine());
            query->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
            return query;
        }
    }
    m_Logger->logError("Cannot create Query engine");
    return QSharedPointer<QueryEngine>();
}

QSharedPointer<ITagProcessor> EDocFactory::createTagEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "ITagEngine *EDocFactory::createTagEngine()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("tagengine");
        if (conf)
        {
            QString engineClass = ((XMLElement*)conf->get("class"))->value();

            QPluginLoader pluginLoader(tagPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                ITagProcessor *engine = qobject_cast<ITagProcessor*>(plugin);
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return QSharedPointer<ITagProcessor>(qobject_cast<ITagProcessor *>(plugin));
            }
        }
    }
    m_Logger->logError("Cannot create database engine");
    return QSharedPointer<ITagProcessor>();
}

QSharedPointer<IServer> EDocFactory::createServerEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IServer *EDocFactory::createServerEngine()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("server");
        if (conf)
        {
            QString engineClass = ((XMLElement*)conf->get("class"))->value();

            QPluginLoader pluginLoader(serverPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IServer *engine = qobject_cast<IServer*>(plugin);
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return QSharedPointer<IServer>(qobject_cast<IServer *>(plugin));
            }
            else
            {
                m_Logger->logError(pluginLoader.errorString());
            }
        }
    }
    m_Logger->logError("Cannot create server engine");
    return QSharedPointer<IServer>();
}

QSharedPointer<IRecord> EDocFactory::createEmptyRecord()
{
    return databaseEngine()->createEmptyRecord();
}

void EDocFactory::addDocument(const QString &filename, QSharedPointer<IRecord> record)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray blob = file.readAll();
    addDocumentFromBlob(blob, filename, record);
    file.close();
}

void EDocFactory::addDocumentFromBlob(QByteArray &blob, const QString &filename, QSharedPointer<IRecord> record)
{
    QSharedPointer<IDocID> docId = engine->addDocument(blob);
//    IDocument *doc = (IDocument*)engine->getDocument(docId);

    record->value("archivo")->setValue(docId->asString());
    record->value("filename")->setValue(filename);

    QSharedPointer<IRecordID> record_id = databaseEngine()->addRecord(record);

    if (!record->value("keywords")->isNull())
    {
        tagEngine()->processKeywordString(record_id, record->value("keywords")->content().toString());
    }
}
