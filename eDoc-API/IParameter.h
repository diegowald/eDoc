#ifndef IPARAMETER_H
#define IPARAMETER_H

#include "APIEnums.h"
#include "IFieldDefinition.h"
#include "IValue.h"

struct IParameter
{
public:
    virtual void setField(QSharedPointer<IFieldDefinition> field) = 0;
    virtual QSharedPointer<IFieldDefinition> field() = 0;
    virtual void addValue(QSharedPointer<IValue> value) = 0;
    virtual QList<QSharedPointer<IValue> > values() = 0;
    virtual void setQueryType(VALIDQUERY queryType) = 0;
    virtual VALIDQUERY queryType() = 0;
    virtual ~IParameter() {}
};

#endif // IPARAMETER_H
