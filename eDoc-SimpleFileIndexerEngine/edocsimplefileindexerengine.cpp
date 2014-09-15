#include "edocsimplefileindexerengine.h"


EDocSimpleFileIndexerEngine::EDocSimpleFileIndexerEngine(QObject *parent)
    : QObject(parent)
{
}

EDocSimpleFileIndexerEngine::~EDocSimpleFileIndexerEngine()
{
}

void EDocSimpleFileIndexerEngine::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    m_SQLManager.initialize(configuration, factory);
}

IDocBasePtr EDocSimpleFileIndexerEngine::createDocument(const QByteArray& blob)
{
}

IDocBasePtr EDocSimpleFileIndexerEngine::createDocument(const QString sourcePath, const QByteArray &blob)
{
//    QString SQL = "INSERT INTO FileNames (record_"
}

IDocBasePtr EDocSimpleFileIndexerEngine::getDocument(IDocIDPtr id)
{
}

IDocBasePtr EDocSimpleFileIndexerEngine::getDocument(const QString &id)
{
}

bool EDocSimpleFileIndexerEngine::deleteDocument(IDocIDPtr id)
{
}

IDocIDPtr EDocSimpleFileIndexerEngine::IValueToIDocId(IValuePtr value)
{
}

QString EDocSimpleFileIndexerEngine::name()
{
    return "EDocSimpleFileIndexerEngine";
}

IDocEnginePtr EDocSimpleFileIndexerEngine::newDocEngine()
{
    return IDocEnginePtr(new EDocSimpleFileIndexerEngine());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-SimpleFileIndexerEngine, EDocSimpleFileIndexerEngine)
#endif // QT_VERSION < 0x050000
