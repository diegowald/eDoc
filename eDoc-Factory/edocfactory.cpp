#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>
#include <QsLog.h>
#include "configreader.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/xmlelement.h"

EDocFactory::EDocFactory() :
    pluginPath(""), xmlFile(""),
    plugins()
{
    QsLogging::Logger & logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);

    const QString logPath("./eDoc-Factory.log");


    // Create log destinations
    QsLogging::DestinationPtr fileDestination(
       QsLogging::DestinationFactory::MakeFileDestination(logPath) );
    QsLogging::DestinationPtr debugDestination(
       QsLogging::DestinationFactory::MakeDebugOutputDestination() );

    // set log destinations on the logger
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    QLOG_TRACE() << "EDocFactory::EDocFactory()";
}

EDocFactory::~EDocFactory()
{
    QLOG_TRACE() << "EDocFactory::~EDocFactory()";
}

void EDocFactory::readAvailablePlugins()
{
    QLOG_TRACE() << "void EDocFactory::readAvailablePlugins()";
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
                QLOG_DEBUG() << "Engine Name: " << engine->name() << ", File: " << f;
            }
        }
        delete plugin;
    }
}

void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile)
{
    QLOG_TRACE() << "void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile)";
    this->xmlFile = xmlFile;
    this->pluginPath = pluginPath;
    readAvailablePlugins();

    // Ahora a leer e instanciar y configurar el plugin
    ConfigReader reader(this->xmlFile);
    configuration = reader.getConfiguration();
    QLOG_TRACE() << configuration->toDebugString();

    engine = createEngine();
}

IDocEngine* EDocFactory::docEngine()
{
    QLOG_TRACE() << "IDocEngine* EDocFactory::docEngine()";
    return engine;
}

IDocEngine *EDocFactory::createEngine()
{
    QLOG_TRACE() << "IDocEngine *EDocFactory::createEngine()";

    if (configuration->key() == "engine")
    {
        XMLCollection *conf = (XMLCollection*) configuration;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        QPluginLoader pluginLoader(plugins[engineClass]);
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            return qobject_cast<IDocEngine *>(plugin);
        }
    }
    return NULL;
}
