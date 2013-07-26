#include "inmemorytagprocessor.h"
#include <QStringList>

InMemoryTagProcessor::InMemoryTagProcessor(QObject *parent) :
    QObject(parent)
{
}
void InMemoryTagProcessor::initialize(QObjectLogging *logger)
{
    m_Logger = logger;
}

void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)");
    QStringList tags = tag->keys();
    foreach (QString tagString, tags)
    {
        m_Tag[tagString].insert(recordID);
    }
}

QSet<IRecordID*> InMemoryTagProcessor::findByTags(const QStringList &tags)
{
    m_Logger->logTrace(__FILE__, __LINE__, "eDoc-InMemoryTagging", "QSet<IRecordID*> InMemoryTagProcessor::findByTags(const QStringList &tags)");
    QSet<IRecordID*> result;
    bool firstTagProcessed = false;
    foreach (QString tag, tags)
    {
        QSet<IRecordID*> partialSet;
        if (m_Tag.contains(tag))
            partialSet = m_Tag[tag];

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
            m_Tag[tagLabel].remove(recordID);
            if (0 == m_Tag[tagLabel].count())
                m_Tag.remove(tagLabel);
        }
    }
}
