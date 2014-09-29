#include "document.h"
#include <QFile>
#include <QFileInfo>

Document::Document(IDocIDPtr id, const QString &location, QObject *parent) :
    QObject(parent), fileLocation(location), _id(id)
{
}

Document::~Document()
{
}

IDocIDPtr Document::id()
{
    return _id;
}

QByteArray Document::blob()
{
    QByteArray result;
    QFile file(fileLocation);
    if (file.exists())
    {
        result = file.readAll();
    }
    return result;
}

long Document::size()
{
    QFileInfo fi(fileLocation);
    return fi.size();
}
