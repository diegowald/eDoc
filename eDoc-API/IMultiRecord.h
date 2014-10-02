#ifndef IMULTIRECORD_H
#define IMULTIRECORD_H

#include "forward.h"
#include <QMetaType>
#include "IRecordID.h"
#include "IRecord.h"
#include "IFieldDefinition.h"
#include "IValue.h"
#include <QList>

class IMultiRecord
{
public:
    virtual void setID(IRecordIDPtr ID) = 0;
    virtual IRecordIDPtr ID() = 0;
    virtual void addRecord(IRecordPtr record) = 0;
    virtual IRecordPtr getRecord(IRecordIDPtr recordID) = 0;
    virtual QList<IRecordPtr> getRecords() = 0;
    virtual void removeRecord(IRecordIDPtr recordID) = 0;
    virtual bool containsRecord(IRecordIDPtr recordID) = 0;
    virtual ~IMultiRecord() {}
};

Q_DECLARE_METATYPE(IMultiRecordPtr)

#endif // IMULTIRECORD_H
