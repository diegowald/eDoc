#include "fileengine.h"
#include <QFile>
#include "simplefileid.h"
#include <../eDoc-Configuration/xmlelement.h>
#include <../eDoc-Configuration/xmlcollection.h>
#include "simplefiledocument.h"
#include "../eDoc-API/IFactory.h"

FileEngine::FileEngine(QObject *parent) : QObject(parent)
{
}

FileEngine::~FileEngine()
{
}

void FileEngine::initialize(IXMLContentPtr configuration, IFactory* factory)
{
    folder = configuration.dynamicCast<XMLCollection>()->get("folder").dynamicCast<XMLElement>()->value();
    m_Logger = factory->logger();
    fileManager = QSharedPointer<FileManagement>(new FileManagement(folder));
}

IDocBasePtr FileEngine::createDocument(const QString sourcePath, const QByteArray &blob)
{
    (void) sourcePath;
    return createDocument(blob);
}

IDocBasePtr FileEngine::createDocument(const QByteArray& blob)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocID* FileEngine::createDocument(const QByteArray& blob)");
    QSharedPointer<SimpleFileID> id = QSharedPointer<SimpleFileID>(new SimpleFileID());

    fileManager->createFile(id->asString(), blob);

    return getDocument(id);
}

IDocBasePtr FileEngine::getDocument(IDocIDPtr id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocBase* FileEngine::getDocument(IDocID *id) const");
    QSharedPointer<SimpleFileDocument> doc = QSharedPointer<SimpleFileDocument>(new SimpleFileDocument(fileManager, id->asString()));
    return doc;
}

IDocBasePtr FileEngine::getDocument(const QString &id)
{
    QSharedPointer<SimpleFileDocument> doc = QSharedPointer<SimpleFileDocument>(new SimpleFileDocument(fileManager, id));
    return doc;
}

bool FileEngine::deleteDocument(IDocIDPtr id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "bool FileEngine::deleteDocument(IDocID *id)");
    return false;
}

IDocIDPtr FileEngine::IValueToIDocId(IValuePtr value)
{
    return QSharedPointer<IDocID>(new SimpleFileID(value->content().toString()));
}

QString FileEngine::name()
{
    return "SimpleFileEngine";
}

IDocEnginePtr FileEngine::newDocEngine()
{
    return IDocEnginePtr(new FileEngine());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(SimpleFileEngine, FileEngine)
#endif // QT_VERSION < 0x050000
