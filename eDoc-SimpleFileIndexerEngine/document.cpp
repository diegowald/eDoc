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
        file.open(QIODevice::ReadOnly);
        result = file.readAll();
        file.close();
    }
    return result;
}

long Document::size()
{
    QFileInfo fi(fileLocation);
    return fi.size();
}
