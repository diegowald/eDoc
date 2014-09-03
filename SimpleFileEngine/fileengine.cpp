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

void FileEngine::initialize(QSharedPointer<IXMLContent> configuration,
                            QSharedPointer<QObjectLogging> logger,
                            const QMap<QString, QString> &docpluginStock,
                            const QMap<QString, QString> &DBplugins,
                            const QMap<QString, QString> &DBWithHistoryPlugins,
                            const QMap<QString, QString> &tagPlugins,
                            const QMap<QString, QString> &serverPlugins)
{
    (void)docpluginStock;
    (void)DBplugins;
    (void)DBWithHistoryPlugins;
    (void)tagPlugins;
    (void)serverPlugins;
    folder = configuration.dynamicCast<XMLCollection>()->get("folder").dynamicCast<XMLElement>()->value();
    m_Logger = logger;
    fileManager = QSharedPointer<FileManagement>(new FileManagement(folder));
}

QSharedPointer<IDocID> FileEngine::addDocument(const QByteArray& blob)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocID* FileEngine::addDocument(const QByteArray& blob)");
    QSharedPointer<SimpleFileID> id = QSharedPointer<SimpleFileID>(new SimpleFileID());

    fileManager->createFile(id->asString(), blob);

    return id;
}

QSharedPointer<IDocBase> FileEngine::getDocument(QSharedPointer<IDocID> id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "IDocBase* FileEngine::getDocument(IDocID *id) const");
    QSharedPointer<SimpleFileDocument> doc = QSharedPointer<SimpleFileDocument>(new SimpleFileDocument(fileManager, id->asString()));
    return doc;
}

bool FileEngine::deleteDocument(QSharedPointer<IDocID> id)
{
    //m_Logger->logTrace(__FILE__, __LINE__, "FileEngine", "bool FileEngine::deleteDocument(IDocID *id)");
    return false;
}

QSharedPointer<IDocID> FileEngine::IValueToIDocId(QSharedPointer<IValue> value)
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
