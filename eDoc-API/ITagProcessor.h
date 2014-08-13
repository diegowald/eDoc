#ifndef ITAGPROCESSOR_H
#define ITAGPROCESSOR_H

#include "ITag.h"
#include "IRecordID.h"
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"

struct ITagProcessor
{
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger, const QMap<QString, QString> &pluginStock) = 0;
    virtual void addTagRecord(IRecordID *recordID, ITag* tag) = 0;
    virtual QSet<QString> findByTags(const QStringList &tags) = 0;
    virtual void removeRecord(IRecordID* recordID, ITag* tag) = 0;
    virtual void processKeywordString(IRecordID *recordID, const QString &keywords) = 0;
    virtual QString name() = 0;

    virtual ~ITagProcessor() {}
};

Q_DECLARE_INTERFACE(ITagProcessor, "com.mksingenieria.eDoc.ITagProcessor/0.0")
#endif // ITAGPROCESSOR_H
