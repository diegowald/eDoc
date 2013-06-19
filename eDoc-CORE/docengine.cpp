#include "docengine.h"

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

IDocID* DocEngine::addDocument()
{
    DocID *id = DocID::createNew();
    Document *doc = new Document(id);
    collection[*id] = doc;
    return id;
}

IDocument* DocEngine::getDocument(IDocID *id) const
{
    DocID *iid = (DocID*)(id);
    return collection[*iid];
}

bool DocEngine::deleteDocument(IDocID *id)
{
    collection.remove(*(DocID*)id);
    return true;
}
