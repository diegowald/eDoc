#include "edocsimplefileindexerengine.h"
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../SimpleFileEngine/simplefileid.h"
#include "document.h"

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
    tableName = configuration.dynamicCast<XMLCollection>()->get("tablename").dynamicCast<XMLElement>()->value();
}

IDocBasePtr EDocSimpleFileIndexerEngine::createDocument(const QByteArray& blob)
{
}

IDocBasePtr EDocSimpleFileIndexerEngine::createDocument(const QString sourcePath, const QByteArray &blob)
{
    QString SQLInsert = "INSERT INTO %1 (record_id, filename) VALUES (record_id, id_file, filename);";
    QString sql = SQLInsert.arg(tableName);

    DBRecordPtr record = DBRecordPtr(new DBRecord());

    IDocIDPtr id(new SimpleFileID());
    (*record)["id_file"] = id->asString();
    (*record)["filename"] = sourcePath;

    m_SQLManager.executeCommand(sql, record);
    return getDocument(id);
}

IDocBasePtr EDocSimpleFileIndexerEngine::getDocument(IDocIDPtr id)
{    
    return getDocument(id->asString());
}

IDocBasePtr EDocSimpleFileIndexerEngine::getDocument(const QString &id)
{
    QString SQLSelect = "SELECT * FROM %1 WHERE record_id = :record_id";
    QString sql = SQLSelect.arg(tableName);
    DBRecordPtr record = DBRecordPtr(new DBRecord());

    (*record)["id_file"] = id;

    DBRecordSet recs = m_SQLManager.getRecords(sql, record);

    IDocumentPtr doc;
    if (recs->count() > 0)
    {
        IDocIDPtr docId(new SimpleFileID(id));
        record = recs->at(0);
        QString location((*record)["filename"].toString());
        doc = IDocumentPtr(new Document(docId, location));
    }
    return doc;
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
