#include "document.h"
#include "../eDoc-API/IDocument.h"

Document::Document(DocIDPtr id, const QString &sourcePath, QObject *parent) :
    QObject(parent), iid(id), originalSourcePath(sourcePath)
{
}

Document::Document(DocIDPtr id, QObject *parent) : QObject(parent), iid(id), originalSourcePath("")
{
}

Document::~Document()
{
}

IDocIDPtr Document::id()
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

