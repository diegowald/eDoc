#include "simplefiledocument.h"
#include <QFile>


SimpleFileDocument::SimpleFileDocument(QSharedPointer<FileManagement> FileManager, QObject *parent) :
    QObject(parent), fileManager(FileManager)
{
    idDocument = QSharedPointer<SimpleFileID>(new SimpleFileID(this));
}

SimpleFileDocument::SimpleFileDocument(QSharedPointer<FileManagement> FileManager, QString stringID, QObject *parent) :
    QObject(parent), fileManager(FileManager)
{
    idDocument = QSharedPointer<SimpleFileID>(new SimpleFileID(stringID,this));
}

SimpleFileDocument::~SimpleFileDocument()
{
}

QSharedPointer<IDocID> SimpleFileDocument::id()
{
    return idDocument;
}

QByteArray SimpleFileDocument::blob()
{
    return fileManager->readFile(idDocument->asString());
}
