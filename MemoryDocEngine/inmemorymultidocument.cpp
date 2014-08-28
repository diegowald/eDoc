#include "inmemorymultidocument.h"
#include "inmemorydocument.h"


InMemoryMultiDocument::InMemoryMultiDocument(QSharedPointer<IMultiDocument> persistentDoc, QObject *parent) :
    QObject(parent)
{
    m_PersistentDocument = persistentDoc;
    foreach (QSharedPointer<IDocBase> docB, persistentDoc->getDocuments()) {
        if (docB->isComplex())
            m_Collection[docB->id()->asString()] = QSharedPointer<IDocBase>(new InMemoryMultiDocument(docB.dynamicCast<IMultiDocument>(), this));
        else
            m_Collection[docB->id()->asString()] = QSharedPointer<IDocBase>(new InMemoryDocument(docB.dynamicCast<IDocument>(), this));
    }
}

InMemoryMultiDocument::~InMemoryMultiDocument()
{
}

QSharedPointer<IDocID> InMemoryMultiDocument::id()
{
    return m_PersistentDocument->id();
}

void InMemoryMultiDocument::addDocument(QSharedPointer<IDocBase> doc)
{
    m_Collection[doc->id()->asString()] = doc;
}

QSharedPointer<IDocBase> InMemoryMultiDocument::getDocument(QSharedPointer<IDocID> id)
{
    if (!m_Collection.contains(id->asString()))
    {
        if (m_PersistentDocument->containsDocument(id))
        {
            QSharedPointer<IDocBase> doc = m_PersistentDocument->getDocument(id);
            if (doc->isComplex())
                m_Collection[id->asString()] = QSharedPointer<IDocBase>(new InMemoryMultiDocument(doc.dynamicCast<IMultiDocument>(), this));
            else
                m_Collection[id->asString()] = QSharedPointer<IDocBase>(new InMemoryDocument(doc.dynamicCast<IDocument>(), this));
        }
    }
    if (m_Collection.contains(id->asString()))
        return m_Collection[id->asString()];
    else
        return QSharedPointer<IDocBase>(); // throw exception???
}

QList<QSharedPointer<IDocBase> > InMemoryMultiDocument::getDocuments()
{
    return m_Collection.values();
}

void InMemoryMultiDocument::removeDocument(QSharedPointer<IDocID> id)
{
    if (m_Collection.contains(id->asString()))
    {
        m_Collection.remove(id->asString());
        m_PersistentDocument->removeDocument(id);
    }
}

bool InMemoryMultiDocument::containsDocument(QSharedPointer<IDocID> id)
{
    return m_Collection.contains(id->asString());
}
