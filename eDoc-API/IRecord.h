#ifndef IRECORD_H
#define IRECORD_H

#include "forward.h"
#include <QMetaType>
#include "IRecordID.h"
#include "IFieldDefinition.h"
#include "IValue.h"

class IRecord
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

Q_DECLARE_METATYPE(IRecordPtr)

#endif // IRECORD_H
