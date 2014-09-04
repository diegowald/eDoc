#include "document.h"

Document::Document(QSharedPointer<DocID> id, QObject *parent) :
    QObject(parent)
{
    iid = id;
}

QSharedPointer<IDocID> Document::id()
{
    return iid;
}

QByteArray Document::blob()
{
    return QByteArray();
}

long Document::size()
{
    return 0;
}

