#include "inmemorytagprocessor.h"

InMemoryTagProcessor::InMemoryTagProcessor(QObject *parent) :
    QObject(parent)
{
}

void InMemoryTagProcessor::addTagRecord(IRecordID *recordID, ITag* tag)
{
    QStringList tags = tag->getTagList();
    foreach (QString tagString, tags)
    {
        m_Tag[tagString].insert(recordID);
    }
}

QSet<IRecordID*> InMemoryTagProcessor::findByTags(const QList<QString> &tags)
{
}

void InMemoryTagProcessor::removeRecord(IRecordID* recordID, ITag* tag)
{
}
