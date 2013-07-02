#include "memorydocengine.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>
#include <QPluginLoader>

MemoryDocEngine::MemoryDocEngine(QObject *parent) :
    QObject(parent)
{
}

MemoryDocEngine::~MemoryDocEngine()
{
}

void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    maxCachedFiles = ((XMLElement*)((XMLCollection*) configuration)->get("maxCachedFiles"))->value().toInt();
    m_Logger = logger;
    m_Logger->logTrace("void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");
    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    persistentEngine = createPersistentEngine(confEngine, pluginStock);
}

IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)
{
    m_Logger->logTrace("IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)");
    return persistentEngine->addDocument(blob);
}

IDocument *MemoryDocEngine::getDocument(IDocID *id)
{
    m_Logger->logTrace("IDocument *MemoryDocEngine::getDocument(IDocID *id)");
    if (!m_Cache.contains(id->asString()))
    {
        IDocument *persistentDoc = persistentEngine->getDocument(id);
        InMemoryDocument *doc = new InMemoryDocument(persistentDoc, NULL);
        QString idString = id->asString();
        m_Cache.insert(idString, doc);
        //m_Cache[idString] = doc;
    }
    return m_Cache[id->asString()];
}

bool MemoryDocEngine::deleteDocument(IDocID *id)
{
    m_Logger->logTrace("bool MemoryDocEngine::deleteDocument(IDocID *id)");
}


QString MemoryDocEngine::name()
{
    return "MemoryDocEngine";
}


IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)
{
    m_Logger->logTrace("IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine, const QMap<QString, QString> &pluginStock)");

    if (confEngine->key() == "engine")
    {
        XMLCollection *conf = (XMLCollection*) confEngine;
        QString engineClass = ((XMLElement*)conf->get("class"))->value();

        if (pluginStock.contains(engineClass)) {
            QPluginLoader pluginLoader(pluginStock[engineClass]);
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                IDocEngine * engine = qobject_cast<IDocEngine*>(plugin);
                engine->initialize(confEngine, m_Logger, pluginStock);
                return qobject_cast<IDocEngine *>(plugin);
            }
            else {
                m_Logger->logError("Plugin: " + engineClass + " cannot be created.");
            }
        }
        else {
            m_Logger->logError("Plugin: " + engineClass + " does not exist.");
        }

    }
    return NULL;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(MemoryDocEngine, MemoryDocEngine)
#endif // QT_VERSION < 0x050000

