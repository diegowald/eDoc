#include "inmemorytagprocessor.h"
#include <QStringList>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "boost/make_shared.hpp"

InMemoryTagProcessor::InMemoryTagProcessor(QObject *parent) :
    QObject(parent), m_SQLManager(this)
{
    maxIdUsed = -1;
}

InMemoryTagProcessor::~InMemoryTagProcessor()
{
}

void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger,
                                      const QMap<QString, QString> &docpluginStock,
                                      const QMap<QString, QString> &DBplugins,
                                      const QMap<QString, QString> &tagPlugins,
                                      const QMap<QString, QString> &serverPlugins)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = ((XMLElement*)((XMLCollection*) configuration)->get("name"))->value();

    m_keywordsTableName = ((XMLElement*)((XMLCollection*)configuration)->get("keywordtablename"))->value();
    m_indexTableName = ((XMLElement*)((XMLCollection*)configuration)->get("indextablename"))->value();
    m_SQLManager.initialize(configuration, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
    loadIntoMemory();
}

void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)");
    QStringList tags = tag->keys();
    processKeywordString(recordID, tags);
}

void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QString &keywords)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QString &keywords)");
    QString s = keywords;
    s = s.replace(".", " ").replace(",", " ");
    QStringList tags = s.split(' ', QString::SkipEmptyParts);
    processKeywordString(recordID, tags);
}

void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QStringList &keywords)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QStringList &keywords)");
    foreach (QString tagString, keywords)
    {
        if (!m_Tag.contains(tagString))
        {
            maxIdUsed++;
            m_Tag[tagString].id = maxIdUsed;
            m_Tag[tagString].saved = false;
        }
        m_Tag[tagString].occurrences.insert(recordID->asString());
        saveKeyword(recordID, tagString);
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
            partialSet = m_Tag[tag].occurrences;

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
            m_Tag[tagLabel].occurrences.remove(recordID->asString());
            if (0 == m_Tag[tagLabel].occurrences.count())
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
        m_Tag[word].id = id;
        m_Tag[word].saved = true;
        index[id] = word;
        maxIdUsed = (maxIdUsed < id) ? id : maxIdUsed;
    }

    SQL = "SELECT keyword_id, record_id from %1";
    sql = SQL.arg(this->m_indexTableName);

    rs = m_SQLManager.getRecords(sql);

    foreach (DBRecordPtr rec, *rs)
    {
        int id((*rec)["keyword_id"].toInt());
        m_Tag[index[id]].occurrences.insert((*rec)["record_id"].toString());
    }
}

void InMemoryTagProcessor::saveKeyword(IRecordID *recordID, const QString &keyword)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::saveKeyword(const int keyword_id)");

    DBRecordPtr keywordRecord = boost::make_shared<DBRecord>();
    // Borro el keyword y sus occurrences
    QString SQL = "DELETE from %1 WHERE keyword_id = :keyword_id AND record_id = :record_id";
    (*keywordRecord)["keyword_id"] = m_Tag[keyword].id;
    (*keywordRecord)["record_id"] = recordID->asString();
    QString sql = SQL.arg(m_indexTableName);
    m_SQLManager.executeCommand(sql, keywordRecord);

    if (!m_Tag[keyword].saved)
    {
        SQL = "INSERT into %1 (keyword_id, word) VALUES (:keyword_id, :word);";
        sql = SQL.arg(m_keywordsTableName);

        DBRecordPtr record = boost::make_shared<DBRecord>();
        (*record)["keyword_id"] = m_Tag[keyword].id;
        (*record)["word"] = keyword;
        m_SQLManager.executeCommand(sql, record);
        m_Tag[keyword].saved = true;
    }

    DBRecordPtr recordOccurrence = boost::make_shared<DBRecord>();
    QString SQLOccurrence = "INSERT INTO %1 (keyword_id, record_id) VALUES (:keyword_id, :record_id);";
    QString sql2 = SQLOccurrence.arg(m_indexTableName);

    (*recordOccurrence)["keyword_id"] = m_Tag[keyword].id;
    (*recordOccurrence)["record_id"] = recordID->asString();

    m_SQLManager.executeCommand(sql2, recordOccurrence);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-InMemoryTagging, InMemoryTagProcessor)
#endif // QT_VERSION < 0x050000

