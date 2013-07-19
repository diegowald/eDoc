#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>
#include "configreader.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"

EDocFactory::EDocFactory() :
    pluginPath(""), xmlFile(""),
    plugins(), DBplugins()
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
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
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
        }
        delete plugin;
    }
}

void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile, QObjectLogging *logger)
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

}

IDocEngine* EDocFactory::docEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine* EDocFactory::docEngine()");
    return engine;
}

IDatabase* EDocFactory::databaseEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase* EDocFactory::databaseEngine()");
    return database;
}

IDocEngine *EDocFactory::createEngine()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDocEngine *EDocFactory::createEngine()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("engine");
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        QPluginLoader pluginLoader(plugins[engineClass]);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IDocEngine * engine = qobject_cast<IDocEngine*>(plugin);
            engine->initialize(conf, m_Logger, plugins);
            return qobject_cast<IDocEngine *>(plugin);
        }
    }
    m_Logger->logError("Cannot create Engine");
    return NULL;
}

IDatabase *EDocFactory::createDatabase()
{
    m_Logger->logTrace(__FILE__, __LINE__, "EDocFactory", "IDatabase *EDocFactory::createDatabase()");
    if ("edoc" == configuration->key())
    {
        XMLCollection *c = (XMLCollection*) configuration;
        XMLCollection *conf = (XMLCollection*)c->get("database");
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        QPluginLoader pluginLoader(DBplugins[engineClass]);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IDatabase *engine = qobject_cast<IDatabase*>(plugin);
            engine->initialize(conf, m_Logger, plugins);
            return qobject_cast<IDatabase *>(plugin);
        }
    }
    m_Logger->logError("Cannot create database engine");
    return NULL;
}
