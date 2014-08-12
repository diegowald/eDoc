#include "inmemorytagprocessor.h"
#include <QStringList>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/qobjectlgging.h"

InMemoryTagProcessor::InMemoryTagProcessor(QObject *parent) :
    QObject(parent)
{
}

void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    m_keywordsTableName = ((XMLElement*)((XMLCollection*)configuration)->get("keywordtablename"))->value();
    m_indexTableName = ((XMLElement*)((XMLCollection*)configuration)->get("indextablename"))->value();
    m_SQLManager.initialize(configuration, logger, pluginStock);
    loadIntoMemory();
}

void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)");
    QStringList tags = tag->keys();
    foreach (QString tagString, tags)
    {
        m_Tag[tagString].second.insert(recordID->asString());
    }
}

QSet<QString> InMemoryTagProcessor::findByTags(const QStringList &tags)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "QSet<IRecordID*> InMemoryTagProcessor::findByTags(const QStringList &tags)");
    QSet<QString> result;
    bool firstTagProcessed = false;
    foreach (QString tag, tags)
    {
        QSet<QString> partialSet;
        if (m_Tag.contains(tag))
            partialSet = m_Tag[tag].second;

        // Si no hay
        if (0 == partialSet.count())
            return partialSet;

        if (firstTagProcessed)
            result = result.intersect(partialSet);
        else
        {
            result = partialSet;
            firstTagProcessed = true;
        }

        if (0 == result.count())
            return result;
    }

    return result;
}

void InMemoryTagProcessor::removeRecord(IRecordID* recordID, ITag* tag)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::removeRecord(IRecordID* recordID, ITag* tag)");
    foreach (QString tagLabel, tag->keys())
    {
        if (m_Tag.contains(tagLabel))
        {
            m_Tag[tagLabel].second.remove(recordID->asString());
            if (0 == m_Tag[tagLabel].second.count())
                m_Tag.remove(tagLabel);
        }
    }
}

QString InMemoryTagProcessor::name()
{
    return "InMemoryTagProcessor";
}


void InMemoryTagProcessor::loadIntoMemory()
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::loadIntoMemory()");
    QString SQL = "SELECT keyword_id, word from %1";
    QString sql = SQL.arg(m_keywordsTableName);

    DBRecordSet rs = m_SQLManager.getRecords(sql);

    QMap<int, QString> index;

    foreach (DBRecordPtr rec, *rs)
    {
        QString word((*rec)["word"].toString());
        int id((*rec)["keyword_id"].toInt());
        m_Tag[word].first = id;
        index[id] = word;
    }

    SQL = "SELECT keyword_id, record_id from %1";
    sql = SQL.arg(this->m_indexTableName);

    rs = m_SQLManager.getRecords(sql);

    foreach (DBRecordPtr rec, *rs)
    {
        int id((*rec)["keyword_id"].toInt());
        m_Tag[index[id]].second.insert((*rec)["record_id"].toString());
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-InMemoryTagging, InMemoryTagProcessor)
#endif // QT_VERSION < 0x050000

