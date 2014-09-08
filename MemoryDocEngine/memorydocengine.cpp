#include "memorydocengine.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>
#include <QPluginLoader>
#include "../eDoc-API/IFactory.h"

MemoryDocEngine::MemoryDocEngine(QObject *parent) :
    QObject(parent)
{
}

MemoryDocEngine::~MemoryDocEngine()
{
}

void MemoryDocEngine::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    maxCachedFiles = configuration.dynamicCast<XMLCollection>()->get("maxCachedFiles").dynamicCast<XMLElement>()->value().toInt();
    m_Logger = factory->logger();
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "void MemoryDocEngine::initialize(IXMLContent *configuration, QObjectLgging *logger, const QMap<QString, QString> &pluginStock)");
    QSharedPointer<XMLCollection> confEngine = configuration.dynamicCast<XMLCollection>()->get("engine").dynamicCast<XMLCollection>();

    persistentEngine = factory->createEngine(confEngine);
}

QSharedPointer<IDocID> MemoryDocEngine::addDocument(const QByteArray& blob)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocID* MemoryDocEngine::addDocument(const QByteArray& blob)");
    return persistentEngine->addDocument(blob);
}

QSharedPointer<IDocBase> MemoryDocEngine::getDocument(QSharedPointer<IDocID> id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "IDocument *MemoryDocEngine::getDocument(IDocID *id)");
    if (!m_Cache.contains(id->asString()))
    {
        QSharedPointer<IDocBase> persistentDoc = persistentEngine->getDocument(id);
        QSharedPointer<IDocBase> doc;
        if (persistentDoc->isComplex())
            doc = QSharedPointer<IDocBase>(new InMemoryMultiDocument(persistentDoc.dynamicCast<IMultiDocument>(), this));
        else
            doc = QSharedPointer<IDocBase>(new InMemoryDocument(persistentDoc.dynamicCast<IDocument>(), this));
        QString idString = id->asString();
        m_Cache.insert(idString, doc);
    }
    return m_Cache[id->asString()];
}

bool MemoryDocEngine::deleteDocument(QSharedPointer<IDocID> id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "MemoryDocEngine", "bool MemoryDocEngine::deleteDocument(IDocID *id)");
}


QString MemoryDocEngine::name()
{
    return "MemoryDocEngine";
}

QSharedPointer<IDocID> MemoryDocEngine::IValueToIDocId(QSharedPointer<IValue> value)
{
    return persistentEngine->IValueToIDocId(value);
}

IDocEnginePtr MemoryDocEngine::newDocEngine()
{
    return IDocEnginePtr(new MemoryDocEngine());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(MemoryDocEngine, MemoryDocEngine)
#endif // QT_VERSION < 0x050000

