#ifndef ITAGPROCESSOR_H
#define ITAGPROCESSOR_H

#include "ITag.h"
#include "IRecordID.h"
#include "IInitializable.h"

struct ITagProcessor : public IInitializable
{
    virtual void addTagRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag) = 0;
    virtual QSet<QString> findByTags(const QStringList &tags) = 0;
    virtual void removeRecord(QSharedPointer<IRecordID> recordID, QSharedPointer<ITag> tag) = 0;
    virtual void processKeywordString(QSharedPointer<IRecordID> recordID, const QString &keywords) = 0;
    virtual QString name() = 0;

    virtual ~ITagProcessor() {}
};

Q_DECLARE_INTERFACE(ITagProcessor, "com.mksingenieria.eDoc.ITagProcessor/0.0")
#endif // ITAGPROCESSOR_H
