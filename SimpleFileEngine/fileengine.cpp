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

void FileEngine::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    folder = ((XMLElement*)((XMLCollection*) configuration)->get("folder"))->value();
    m_Logger = logger;
    fileManager = new FileManagement(folder, this);
}

IDocID* FileEngine::addDocument(const QByteArray& blob)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocID* FileEngine::addDocument(const QByteArray& blob)");
    SimpleFileID *id = new SimpleFileID();

    fileManager->createFile(id->asString(), blob);

    return id;
}

IDocBase *FileEngine::getDocument(IDocID *id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocBase* FileEngine::getDocument(IDocID *id) const");
    SimpleFileDocument *doc = new SimpleFileDocument(fileManager, id->asString(), this);
    return doc;
}

bool FileEngine::deleteDocument(IDocID *id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "bool FileEngine::deleteDocument(IDocID *id)");
    return false;
}

IDocID* FileEngine::IValueToIDocId(IValue *value)
{
    return new SimpleFileID(value->content().toString());
}

QString FileEngine::name()
{
    return "SimpleFileEngine";
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SimpleFileEngine, FileEngine)
#endif // QT_VERSION < 0x050000
