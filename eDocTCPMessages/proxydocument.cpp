#include "proxydocument.h"
#include "../eDoc-API/IDocID.h"

ProxyDocument::ProxyDocument(IDocIDPtr docId, QObject *parent) :
    QObject(parent)
{
    _docId = docId;
}

ProxyDocument::~ProxyDocument()
{
}

IDocIDPtr ProxyDocument::id()
{
    return _docId;
}

bool ProxyDocument::isComplex() const
{
}

long ProxyDocument::size()
{
}
