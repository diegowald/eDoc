#include "document.h"

Document::Document(DocID* id, QObject *parent) :
    QObject(parent)
{
    iid = id;
}

IDocID *Document::id()
{
    return iid;
}

QByteArray Document::blob()
{
    return QByteArray();
}

//Q_EXPORT_PLUGIN2(edoccore, Document)
