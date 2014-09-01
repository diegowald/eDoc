#ifndef IRECORD_H
#define IRECORD_H


#include <QMetaType>
#include "IRecordID.h"
#include "IFieldDefinition.h"
#include "IValue.h"

struct IRecord
{
public:
    virtual void setID(IRecordIDPtr ID) = 0;
    virtual IRecordIDPtr ID() = 0;
    virtual IValuePtr value(IFieldDefinitionPtr field) = 0;
    virtual IValuePtr value(const QString &fieldName) = 0;
    virtual IFieldDefinitionPtr fieldDefinition(const QString &fieldName) = 0;
    virtual QList<QString> fieldNames() = 0;
    virtual ~IRecord() {}
};

typedef QSharedPointer<IRecord> IRecordPtr;
Q_DECLARE_METATYPE(IRecordPtr)

#endif // IRECORD_H
