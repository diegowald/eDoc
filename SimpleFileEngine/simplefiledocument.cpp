#include "simplefiledocument.h"
#include <QFile>


SimpleFileDocument::SimpleFileDocument(FileManagement *FileManager, QObject *parent) :
    QObject(parent), fileManager(FileManager)
{
    idDocument = new SimpleFileID(this);
}

SimpleFileDocument::SimpleFileDocument(FileManagement *FileManager, QString stringID, QObject *parent) :
    QObject(parent), fileManager(FileManager)
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
    return fileManager->readFile(idDocument->asString());
}
