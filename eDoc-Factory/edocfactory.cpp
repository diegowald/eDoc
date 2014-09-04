#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>
#include <QSharedPointer>
#include "configreader.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "queryengine.h"
#include "../eDoc-API/IDocument.h"
#include "databasewithhistorywrapper.h"
#include <QProcess>

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

IDocEnginePtr EDocFactory::docEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine* EDocFactory::docEngine()");
    return engine;
}

IDatabaseWithHistoryPtr EDocFactory::databaseEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase* EDocFactory::databaseEngine()");
    return database;
}

IQueryEnginePtr EDocFactory::queryEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IQueryEngine * EDocFactory::queryEngine()");
    return query;
}

ITagProcessorPtr EDocFactory::tagEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "ITagProcessor *EDocFactory::tagEngine()");
    return tagger;
}

IServerPtr EDocFactory::serverEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IServer *EDocFactory::serverEngine()");
    return server;
}

IDocEnginePtr EDocFactory::createEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine *EDocFactory::createEngine()");
    if ("edoc" == configuration->key())
    {
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("engine").dynamicCast<XMLCollection>();
        if (!conf.isNull())
        {
            QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();
            QPluginLoader pluginLoader(plugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                IDocEngine * engineCreator = qobject_cast<IDocEngine*>(plugin);

                IDocEnginePtr engine = engineCreator->newDocEngine();

                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);

                return engine;
            }
            m_Logger->logError("Cannot create Engine " + engineClass);
        }
    }
    m_Logger->logError("Cannot create Engine");
    return QSharedPointer<IDocEngine>();
}

IDatabaseWithHistoryPtr EDocFactory::createDatabase()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase *EDocFactory::createDatabase()");
    if ("edoc" == configuration->key())
    {
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("database").dynamicCast<XMLCollection>();
        if (conf)
        {
            QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();
            if (DBWithHistoryPlugins.contains(engineClass))
            {
                QPluginLoader pluginLoader(DBWithHistoryPlugins[engineClass]);
                QObject *plugin = pluginLoader.instance();
                if (plugin)
                {
                    IDatabaseWithHistory *engineCreator = qobject_cast<IDatabaseWithHistory*>(plugin);

                    IDatabaseWithHistoryPtr engine = engineCreator->newDatabaseWithHistory();

                    engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                    return engine;
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

IDatabaseWithHistoryPtr EDocFactory::createDatabaseWithoutHistory()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabaseWithHistory *EDocFactory::createDatabaseWithoutHistory()");
    if ("edoc" == configuration->key())
    {
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("database").dynamicCast<XMLCollection>();
        if (conf)
        {
            QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();
            QPluginLoader pluginLoader(DBplugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDatabase* engineCreator = qobject_cast<IDatabase*>(plugin);
                IDatabasePtr engine = engineCreator->newDatabase();
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                IDatabaseWithHistoryPtr db = IDatabaseWithHistoryPtr(new DatabaseWithHistoryWrapper(engine, m_Logger));
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
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("queries").dynamicCast<XMLCollection>();
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

ITagProcessorPtr EDocFactory::createTagEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "ITagEngine *EDocFactory::createTagEngine()");
    if ("edoc" == configuration->key())
    {
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("tagengine").dynamicCast<XMLCollection>();

        if (conf)
        {
            QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

            QPluginLoader pluginLoader(tagPlugins[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                ITagProcessor *engineCreator = qobject_cast<ITagProcessor*>(plugin);
                ITagProcessorPtr engine = engineCreator->newTagProcessor();
                engine->initialize(conf, m_Logger, plugins, DBplugins, DBWithHistoryPlugins, tagPlugins, serverPlugins);
                return engine;
            }
        }
    }
    m_Logger->logError("Cannot create database engine");
    return ITagProcessorPtr();
}

QSharedPointer<IServer> EDocFactory::createServerEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IServer *EDocFactory::createServerEngine()");
    if ("edoc" == configuration->key())
    {
        QSharedPointer<XMLCollection> c = configuration.dynamicCast<XMLCollection>();
        QSharedPointer<XMLCollection> conf = c->get("server").dynamicCast<XMLCollection>();
        if (conf)
        {
            QString engineClass = conf->get("class").dynamicCast<XMLElement>()->value();

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

IRecordPtr EDocFactory::createEmptyRecord()
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

    record->value("archivo")->setValue(docId->asString());
    QFileInfo file(filename);
    if (file.exists())
    {
        record->value("filename")->setValue(file.fileName());
    }
    else
    {
        record->value("filename")->setNull();
    }

    QSharedPointer<IRecordID> record_id = databaseEngine()->addRecord(record);

    if (!record->value("keywords")->isNull())
    {
        if (tagEngine() != NULL)
        {
            tagEngine()->processKeywordString(record_id, record->value("keywords")->content().toString());
        }
    }

    if (tagEngine())
    {
        if (file.completeSuffix() == "pdf")
        {
            QProcess process;
            QString cmd = "pdftotext " + filename + " - | grep -o -E '\\w+' | sort -u -f";
            process.start("bash", QStringList() << "-c" << cmd);
            process.waitForBytesWritten();
            process.waitForFinished();
            QString result(process.readAll());
            QStringList x = result.split('\n');
            tagEngine()->processKeywordString(record_id, x.join(' '));
        }
        if (file.completeSuffix() == "txt")
        {
        }
        if (file.completeSuffix() == "doc")
        {
        }
    }
}
