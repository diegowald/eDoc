#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>
#include "configreader.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"

EDocFactory::EDocFactory() :
    pluginPath(""), xmlFile(""),
    plugins()
{
}

EDocFactory::~EDocFactory()
{
}

void EDocFactory::readAvailablePlugins()
{
    m_Logger->logTrace("void EDocFactory::readAvailablePlugins()");
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
                //emit m_Logger->LogDebug("Engine Name: " + engine->name() + ", File: " + f);
            }
        }
        delete plugin;
    }
}

void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile, QObjectLgging *logger)
{
    m_Logger = logger;
    m_Logger->logTrace("void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile)");
    this->xmlFile = xmlFile;
    this->pluginPath = pluginPath;
    readAvailablePlugins();

    // Ahora a leer e instanciar y configurar el plugin
    ConfigReader reader(this->xmlFile);
    configuration = reader.getConfiguration();
    //emit m_Logger->LogTrace(configuration->toDebugString());

    engine = createEngine();
}

IDocEngine* EDocFactory::docEngine()
{
    m_Logger->logTrace("IDocEngine* EDocFactory::docEngine()");
    return engine;
}

IDocEngine *EDocFactory::createEngine()
{
    m_Logger->logTrace("IDocEngine *EDocFactory::createEngine()");

    if (configuration->key() == "engine")
    {
        XMLCollection *conf = (XMLCollection*) configuration;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        QPluginLoader pluginLoader(plugins[engineClass]);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IDocEngine * engine = qobject_cast<IDocEngine*>(plugin);
            engine->initialize(configuration, m_Logger, plugins);
            return qobject_cast<IDocEngine *>(plugin);
        }
    }
    return NULL;
}
