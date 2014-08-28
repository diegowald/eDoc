#ifndef IMULTIRECORD_H
#define IMULTIRECORD_H

#include <QMetaType>
#include "IRecordID.h"
#include "IRecord.h"
#include "IFieldDefinition.h"
#include "IValue.h"
#include <QList>

struct IMultiRecord
{
public:
    virtual void setID(QSharedPointer<IRecordID> ID) = 0;
    virtual QSharedPointer<IRecordID> ID() = 0;
    virtual void addRecord(QSharedPointer<IRecord> record) = 0;
    virtual QSharedPointer<IRecord> getRecord(QSharedPointer<IRecordID> recordID) = 0;
    virtual QList<QSharedPointer<IRecord>> getRecords() = 0;
    virtual void removeRecord(QSharedPointer<IRecordID> recordID) = 0;
    virtual bool containsRecord(QSharedPointer<IRecordID> recordID) = 0;
    virtual ~IMultiRecord() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IMultiRecord>)

#endif // IMULTIRECORD_H
