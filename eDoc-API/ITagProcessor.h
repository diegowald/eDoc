#ifndef ITAGPROCESSOR_H
#define ITAGPROCESSOR_H

#include "forward.h"
#include "ITag.h"
#include "IRecordID.h"
#include "IInitializable.h"

class ITagProcessor : public IInitializable
{
public:
    virtual void addTagRecord(IRecordIDPtr recordID, ITagPtr tag) = 0;
    virtual QSet<QString> findByTags(const QStringList &tags) = 0;
    virtual void removeRecord(IRecordIDPtr recordID, ITagPtr tag) = 0;
    virtual void processKeywordString(IRecordIDPtr recordID, const QString &keywords) = 0;
    virtual void processKeywordStringList(IRecordIDPtr, const QStringList &keywords) = 0;
    virtual void processRecord(IRecordPtr record) = 0;
    virtual QString name() = 0;
    virtual ITagProcessorPtr newTagProcessor() = 0;

    virtual ~ITagProcessor() {}
};


Q_DECLARE_INTERFACE(ITagProcessor, "com.mksingenieria.eDoc.ITagProcessor/0.0")
#endif // ITAGPROCESSOR_H
