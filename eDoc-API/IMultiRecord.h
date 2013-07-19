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
    virtual void setID(IRecordID *ID) = 0;
    virtual IRecordID *ID() = 0;
    virtual void addRecord(IRecord *record) = 0;
    virtual IRecord* getRecord(IRecordID* recordID) = 0;
    virtual QList<IRecord*> getRecords() = 0;
    virtual void removeRecord(IRecordID *recordID) = 0;
    virtual bool containsRecord(IRecordID *recordID) = 0;
    virtual ~IMultiRecord() {}
};

Q_DECLARE_METATYPE(IMultiRecord*)

#endif // IMULTIRECORD_H
