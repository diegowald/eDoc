#ifndef INMEMORYTAGPROCESSOR_H
#define INMEMORYTAGPROCESSOR_H

#include "edoc-inmemorytagging_global.h"
#include <QObject>
#include <QMap>
#include <QSet>
#include "../eDoc-API/IRecordID.h"
#include "../eDoc-API/ITag.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../eDoc-API/ITagProcessor.h"

class EDOCINMEMORYTAGGINGSHARED_EXPORT InMemoryTagProcessor : public QObject, public ITagProcessor
{
    Q_OBJECT
public:
    explicit InMemoryTagProcessor(QObject *parent = 0);

    virtual void initialize(QObjectLogging *logger);
    virtual void addTagRecord(IRecordID *recordID, ITag* tag);
    virtual QSet<IRecordID*> findByTags(const QStringList &tags);
    virtual void removeRecord(IRecordID* recordID, ITag* tag);

signals:
    
public slots:

private:
    QMap<QString, QSet<IRecordID*> > m_Tag;
    QObjectLogging *m_Logger;
};

#endif // INMEMORYTAGPROCESSOR_H
