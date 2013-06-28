#include "simplefiledocument.h"
#include <QFile>


SimpleFileDocument::SimpleFileDocument(QObject *parent) :
    QObject(parent)
{
    idDocument = new SimpleFileID(this);
}

SimpleFileDocument::SimpleFileDocument(QString stringID, QObject *parent) :
    QObject(parent)
{
    idDocument = new SimpleFileID(stringID,this);
}

SimpleFileDocument::~SimpleFileDocument()
{
}

IDocID* SimpleFileDocument::id()
{
    return idDocument;
}

QByteArray SimpleFileDocument::blob()
{
    QFile file(idDocument->asString());
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}
