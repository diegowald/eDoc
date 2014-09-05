#include "inmemorytagprocessor.h"
#include <QStringList>
#include "../eDoc-Configuration/xmlelement.h"
#include "../eDoc-Configuration/xmlcollection.h"
#include "../eDoc-Configuration/qobjectlgging.h"

InMemoryTagProcessor::InMemoryTagProcessor(QObject *parent) :
    QObject(parent), m_SQLManager(this)
{
    maxIdUsed = -1;
}

InMemoryTagProcessor::~InMemoryTagProcessor()
{
}

void InMemoryTagProcessor::initialize(IXMLContentPtr configuration,
                                      QObjectLoggingPtr logger,
                                      const QMap<QString, QString> &docpluginStock,
                                      const QMap<QString, QString> &DBplugins,
                                      const QMap<QString, QString> &DBWithHistoryPlugins,
                                      const QMap<QString, QString> &tagPlugins,
                                      const QMap<QString, QString> &serverPlugins)
{
    m_Logger = logger;
    m_Logger->logTrace(__FILE__, __LINE__, "InMemoryTagProcessor", "void InMemoryTagProcessor::initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock)");
    m_Name = configuration.dynamicCast<XMLCollection>()->get("name").dynamicCast<XMLElement>()->value();

    m_keywordsTableName = configuration.dynamicCast<XMLCollection>()->get("keywordtablename").dynamicCast<XMLElement>()->value();
    m_indexTableName = configuration.dynamicCast<XMLCollection>()->get("indextablename").dynamicCast<XMLElement>()->value();
    m_SQLManager.initialize(configuration, logger, docpluginStock, DBplugins, tagPlugins, serverPlugins);
    loadIntoMemory();
}

void InMemoryTagProcessor::addTagRecord(IRecordIDPtr recordID, ITagPtr tag)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)");
    QStringList tags = tag->keys();
    processKeywordString(recordID, tags);
}

void InMemoryTagProcessor::processKeywordString(IRecordIDPtr recordID, const QString &keywords)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QString &keywords)");
    QString s = keywords;
    s = s.replace(".", " ").replace(",", " ");
    QStringList tags = s.split(' ', QString::SkipEmptyParts);
    processKeywordString(recordID, tags);
}

void InMemoryTagProcessor::processKeywordString(IRecordIDPtr recordID, const QStringList &keywords)
{
    recordsToSave.clear();
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::processKeywordString(IRecordID *recordID, const QStringList &keywords)");
    bool newKeyword = false;
    foreach (QString tagString, keywords)
    {
        newKeyword = !m_Tag.contains(tagString);
        if (newKeyword)
        {
            maxIdUsed++;
            m_Tag[tagString].id = maxIdUsed;
            m_Tag[tagString].saved = false;
        }
        m_Tag[tagString].occurrences.insert(recordID->asString());
        //saveKeyword(recordID, tagString, newKeyword);
        BULKSAVERECORD bsr;
        bsr.record = recordID;
        bsr.tagString = tagString;
        bsr.newElement = newKeyword;
        recordsToSave.append(bsr);
    }
    bulkSave();
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

void InMemoryTagProcessor::removeRecord(IRecordIDPtr recordID, ITagPtr tag)
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

void InMemoryTagProcessor::saveKeyword(IRecordIDPtr recordID, const QString &keyword, bool newKeyword)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::saveKeyword(const int keyword_id)");
/*
    DBRecordPtr keywordRecord = DBRecordPtr(new DBRecord());
    // Borro el keyword y sus occurrences
    QString SQL = "DELETE from %1 WHERE keyword_id = :keyword_id AND record_id = :record_id";
    (*keywordRecord)["keyword_id"] = m_Tag[keyword].id;
    (*keywordRecord)["record_id"] = recordID->asString();
    QString sql = SQL.arg(m_indexTableName);
    m_SQLManager.executeCommand(sql, keywordRecord);
*/
    if (newKeyword)
    {
        if (!m_Tag[keyword].saved)
        {
            QString SQL = "INSERT into %1 (keyword_id, word) VALUES (:keyword_id, :word);";
            QString sql = SQL.arg(m_keywordsTableName);

            DBRecordPtr record = DBRecordPtr(new DBRecord());
            (*record)["keyword_id"] = m_Tag[keyword].id;
            (*record)["word"] = keyword;
            m_SQLManager.executeCommand(sql, record);
            m_Tag[keyword].saved = true;
        }
    }

    DBRecordPtr recordOccurrence = DBRecordPtr(new DBRecord());
    QString SQLOccurrence = "INSERT INTO %1 (keyword_id, record_id) VALUES (:keyword_id, :record_id);";
    QString sql2 = SQLOccurrence.arg(m_indexTableName);

    (*recordOccurrence)["keyword_id"] = m_Tag[keyword].id;
    (*recordOccurrence)["record_id"] = recordID->asString();

    m_SQLManager.executeCommand(sql2, recordOccurrence);
}

void InMemoryTagProcessor::bulkSave()
{
    QList<DBRecordPtr> keywords;

    QString SQL = "INSERT INTO %1 (%2, %3) VALUES (:%2, :%3)";
    QString labelID = "keyword_id";
    QString labelWord = "word";
    SQL = SQL.arg(m_keywordsTableName, labelID, labelWord);
    QSet<QString> uniqueKeywords;
    bool execute = false;
    foreach (BULKSAVERECORD bsr, recordsToSave)
    {
        if (bsr.newElement && !uniqueKeywords.contains(bsr.tagString))
        {
            DBRecordPtr record(new DBRecord());
            (*record)[labelID] = m_Tag[bsr.tagString].id;
            (*record)[labelWord] = bsr.tagString;
            keywords.append(record);
            uniqueKeywords.insert(bsr.tagString);
            execute = true;
        }
    }
    if (execute)
    {
        m_SQLManager.executeBulk(SQL, keywords);
    }

    keywords.clear();
    execute = false;
    QString keyID = "keyword_id";
    QString recID = "record_id";
    SQL = "INSERT INTO %1 (%2, %3) VALUES (:%2, :%3)";
    SQL = SQL.arg(m_indexTableName, keyID, recID);
    foreach (BULKSAVERECORD bsr, recordsToSave)
    {
        DBRecordPtr record(new DBRecord());
        (*record)[keyID] = m_Tag[bsr.tagString].id;
        (*record)[recID] = bsr.record->asString();
        keywords.append(record);
        execute = true;
    }
    if (execute)
    {
        m_SQLManager.executeBulk(SQL, keywords);
    }
    foreach (BULKSAVERECORD bsr, recordsToSave)
    {
        m_Tag[bsr.tagString].saved = true;
    }
    recordsToSave.clear();
}

ITagProcessorPtr InMemoryTagProcessor::newTagProcessor()
{
    return ITagProcessorPtr(new InMemoryTagProcessor());
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(eDoc-InMemoryTagging, InMemoryTagProcessor)
#endif // QT_VERSION < 0x050000

