#include "docengine.h"
#include <QtWidgets>

DocEngine::DocEngine(QObject *parent) :
    QObject(parent)
{
}

DocEngine::~DocEngine()
{
}

void DocEngine::initialize(IXMLContent *configuration)
{
}

QString DocEngine::name()
{
    return "DocEngine";
}

IDocID* DocEngine::addDocument(const QByteArray& blob)
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

//Q_EXPORT_PLUGIN2(edoccore, DocEngine);
