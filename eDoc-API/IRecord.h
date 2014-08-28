#ifndef IRECORD_H
#define IRECORD_H


#include <QMetaType>
#include "IRecordID.h"
#include "IFieldDefinition.h"
#include "IValue.h"

struct IRecord
{
public:
    virtual void setID(QSharedPointer<IRecordID> ID) = 0;
    virtual QSharedPointer<IRecordID> ID() = 0;
    virtual QSharedPointer<IValue> value(QSharedPointer<IFieldDefinition> field) = 0;
    virtual QSharedPointer<IValue> value(const QString &fieldName) = 0;
    virtual QSharedPointer<IFieldDefinition> fieldDefinition(const QString &fieldName) = 0;
    virtual QList<QString> fieldNames() = 0;
    virtual ~IRecord() {}
};

Q_DECLARE_METATYPE(QSharedPointer<IRecord>)

#endif // IRECORD_H
