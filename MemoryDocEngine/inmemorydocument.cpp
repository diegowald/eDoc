#include "inmemorydocument.h"

InMemoryDocument::InMemoryDocument(IDocument* persistentDoc, QObject *parent) :
    QObject(parent)
{
    cached = false;
    m_PersistentDocument = persistentDoc;
}

InMemoryDocument::~InMemoryDocument()
{
    delete m_PersistentDocument;
}

IDocID* InMemoryDocument::id()
{
    return m_PersistentDocument->id();
}

QByteArray InMemoryDocument::blob()
{
    if (!cached)
    {
        cachedData = m_PersistentDocument->blob();
        cached = true;
    }
    return cachedData;
}
