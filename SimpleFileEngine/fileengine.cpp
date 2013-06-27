#include "fileengine.h"
#include <QFile>


FileEngine::FileEngine(QObject *parent)
{
}

FileEngine::~FileEngine()
{
}

IDocID* FileEngine::addDocument(const QByteArray& blob)
{
    QFile file("test");
    file.open(QIODevice::WriteOnly);
    file.write(blob);
    file.close();
}

IDocument* FileEngine::getDocument(IDocID *id) const
{
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
