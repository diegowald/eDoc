#ifndef IRECORD_H
#define IRECORD_H


#include <QMetaType>
#include "IRecordID.h"
#include "IFieldDefinition.h"
#include "IValue.h"

struct IRecord
{
public:
    virtual void setID(IRecordID *ID) = 0;
    virtual IRecordID *ID() = 0;
    virtual IValue* value(IFieldDefinition* field) = 0;
    virtual IValue* value(const QString &fieldName) = 0;
    virtual IFieldDefinition* fieldDefinition(const QString &fieldName) = 0;
    virtual QList<QString> fieldNames() = 0;
    virtual ~IRecord() {}
};

Q_DECLARE_METATYPE(IRecord*)

#endif // IRECORD_H
