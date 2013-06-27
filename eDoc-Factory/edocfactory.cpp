#include "edocfactory.h"
#include <QDir>
#include <QPluginLoader>

EDocFactory::EDocFactory() :
    pluginPath(""), xmlFile(""),
    plugins()
{
}

EDocFactory::~EDocFactory()
{
}

void EDocFactory::initialize(const QString &pluginPath, const QString &xmlFile)
{
    this->xmlFile = xmlFile;
    this->pluginPath = pluginPath;

    QDir pluginsDir(pluginPath);
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QString f = pluginsDir.absoluteFilePath(fileName);
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            IDocEngine* engine = qobject_cast<IDocEngine *>(plugin);
            if (engine)
                plugins[engine->name()] = engine;
        }
    }
}

IDocEngine* EDocFactory::docEngine()
{
    return NULL;
}
