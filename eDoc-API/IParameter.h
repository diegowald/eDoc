#ifndef IPARAMETER_H
#define IPARAMETER_H

#include <APIEnums.h>
#include <IFieldDefinition.h>
#include <IValue.h>

struct IParameter
{
public:
    virtual void setField(IFieldDefinition *field) = 0;
    virtual IFieldDefinition* field() = 0;
    virtual void addValue(IValue *value) = 0;
    virtual QList<IValue*> values() = 0;
    virtual void setQueryType(VALIDQUERY queryType) = 0;
    virtual VALIDQUERY queryType() = 0;
    virtual ~IParameter() {}
};

#endif // IPARAMETER_H
