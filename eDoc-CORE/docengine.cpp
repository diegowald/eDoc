#include "docengine.h"
#include <QtCore/qplugin.h>

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

DocEngine::~DocEngine()
{
}

QString DocEngine::name()
{
    return "DocEngine";
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

Q_EXPORT_PLUGIN2(edoccore, DocEngine);
