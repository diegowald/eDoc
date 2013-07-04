#include "inmemorymultidocument.h"
#include "inmemorydocument.h"


InMemoryMultiDocument::InMemoryMultiDocument(IMultiDocument* persistentDoc, QObject *parent) :
    QObject(parent)
{
    m_PersistentDocument = persistentDoc;
    foreach (IDocBase* docB, persistentDoc->getDocuments()) {
        if (docB->isComplex())
            m_Collection[docB->id()->asString()] = new InMemoryMultiDocument((IMultiDocument*)docB, this);
        else
            m_Collection[docB->id()->asString()] = new InMemoryDocument((IDocument*)docB, this);
    }
}

InMemoryMultiDocument::~InMemoryMultiDocument()
{
}

IDocID *InMemoryMultiDocument::id()
{
    return m_PersistentDocument->id();
}

void InMemoryMultiDocument::addDocument(IDocBase *doc)
{
    m_Collection[doc->id()->asString()] = doc;
}

IDocBase* InMemoryMultiDocument::getDocument(IDocID *id)
{
    if (!m_Collection.contains(id->asString()))
    {
        if (m_PersistentDocument->containsDocument(id))
        {
            IDocBase *doc = m_PersistentDocument->getDocument(id);
            if (doc->isComplex())
                m_Collection[id->asString()] = new InMemoryMultiDocument((IMultiDocument*)doc, this);
            else
                m_Collection[id->asString()] = new InMemoryDocument((IDocument*)doc, this);
        }
    }
    if (m_Collection.contains(id->asString()))
        return m_Collection[id->asString()];
    else
        return NULL; // throw exception???
}

QList<IDocBase*> InMemoryMultiDocument::getDocuments()
{
    return m_Collection.values();
}

void InMemoryMultiDocument::removeDocument(IDocID *id)
{
    if (m_Collection.contains(id->asString()))
    {
        m_Collection.remove(id->asString());
        m_PersistentDocument->removeDocument(id);
    }
}

bool InMemoryMultiDocument::containsDocument(IDocID *id)
{
    return m_Collection.contains(id->asString());
}
