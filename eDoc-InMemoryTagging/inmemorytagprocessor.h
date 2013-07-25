#ifndef INMEMORYTAGPROCESSOR_H
#define INMEMORYTAGPROCESSOR_H

#include "edocinmemorytagging.h"
#include <QObject>
#include <QMap>
#include <QSet>
#include "../eDoc-API/IRecordID.h"
#include "../eDoc-API/ITag.h"
#include "../eDoc-Configuration/qobjectlgging.h"

class EDOCINMEMORYTAGGINGSHARED_EXPORT InMemoryTagProcessor : public QObject
{
    Q_OBJECT
public:
    explicit InMemoryTagProcessor(QObject *parent = 0);
    
    virtual void addTagRecord(IRecordID *recordID, ITag* tag);
    virtual QSet<IRecordID*> findByTags(const QList<QString> &tags);
    virtual void removeRecord(IRecordID* recordID, ITag* tag);
signals:
    
public slots:

private:
    QMap<QString, QSet<IRecordID*> > m_Tag;
};

#endif // INMEMORYTAGPROCESSOR_H
