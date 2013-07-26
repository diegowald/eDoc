#ifndef ITAGPROCESSOR_H
#define ITAGPROCESSOR_H

#include "ITag.h"
#include "IRecordID.h"

struct ITagProcessor
{
    virtual void initialize(QObjectLogging *logger) = 0;
    virtual void addTagRecord(IRecordID *recordID, ITag* tag) = 0;
    virtual QSet<IRecordID*> findByTags(const QStringList &tags) = 0;
    virtual void removeRecord(IRecordID* recordID, ITag* tag) = 0;
};

#endif // ITAGPROCESSOR_H
