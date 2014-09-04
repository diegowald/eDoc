#include "inmemorydocument.h"

InMemoryDocument::InMemoryDocument(QSharedPointer<IDocument> persistentDoc, QObject *parent) :
    QObject(parent)
{
    cached = false;
    m_PersistentDocument = persistentDoc;
}

InMemoryDocument::~InMemoryDocument()
{
}

QSharedPointer<IDocID> InMemoryDocument::id()
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

long InMemoryDocument::size()
{
    long docSize = blob().size();
    return docSize;
}
