#include "fileengine.h"
#include <QFile>
#include "simplefileid.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>
#include "simplefiledocument.h"

FileEngine::FileEngine(QObject *parent) : QObject(parent)
{
}

FileEngine::~FileEngine()
{
}

void FileEngine::initialize(IXMLContent *configuration)
{
    folder = ((XMLElement*)((XMLCollection*) configuration)->get("folder"))->value();
}

IDocID* FileEngine::addDocument(const QByteArray& blob)
{
    SimpleFileID *id = new SimpleFileID();
    QString filename(folder + "/" + id->asString());
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(blob);
    file.close();
    return id;
}

IDocument* FileEngine::getDocument(IDocID *id) const
{
    SimpleFileDocument *doc = new SimpleFileDocument(id->asString());
    return doc;
}

bool FileEngine::deleteDocument(IDocID *id)
{
    return false;
}


QString FileEngine::name()
{
    return "SimpleFileEngine";
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SimpleFileEngine, FileEngine)
#endif // QT_VERSION < 0x050000
