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

void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                 const QMap<QString, QString> &docpluginStock,
                                 const QMap<QString, QString> &DBplugins,
                                 const QMap<QString, QString> &tagPlugins,
                                 const QMap<QString, QString> &serverPlugins)
{
    maxCachedFiles = ((XMLElement*)((XMLCollection*) configuration)->get("maxCachedFiles"))->value().toInt();
    m_Logger = logger;
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");
    XMLCollection *confEngine = (XMLCollection*)((XMLCollection*)configuration)->get("engine");
    persistentEngine = createPersistentEngine(confEngine, docpluginStock, DBplugins, tagPlugins, serverPlugins);
}

IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)");
    return persistentEngine->addDocument(blob);
}

IDocBase *MemoryDocEngine::getDocument(IDocID *id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocument *MemoryDocEngine::getDocument(IDocID *id)");
    if (!m_Cache.contains(id->asString()))
    {
        IDocBase *persistentDoc = persistentEngine->getDocument(id);
        IDocBase *doc = NULL;
        if (persistentDoc->isComplex())
            doc = new InMemoryMultiDocument((IMultiDocument*) persistentDoc, this);
        else
            doc = new InMemoryDocument((IDocument*)persistentDoc, this);
        QString idString = id->asString();
        m_Cache.insert(idString, doc);
        //m_Cache[idString] = doc;
    }
    return m_Cache[id->asString()];
}

bool MemoryDocEngine::deleteDocument(IDocID *id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "bool MemoryDocEngine::deleteDocument(IDocID *id)");
}


QString MemoryDocEngine::name()
{
    return "MemoryDocEngine";
}


IDocEngine *MemoryDocEngine::createPersistentEngine(XMLCollection *confEngine,
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
                IDocEngine * engine = qobject_cast<IDocEngine*>(plugin);
                engine->initialize(confEngine, m_Logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
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

IDocID* MemoryDocEngine::IValueToIDocId(IValue *value)
{
    return persistentEngine->IValueToIDocId(value);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(MemoryDocEngine, MemoryDocEngine)
#endif // QT_VERSION < 0x050000

