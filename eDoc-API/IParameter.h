#ifndef IPARAMETER_H
#define IPARAMETER_H

#include "edocapi_global.h"
#include "forward.h"
#include "APIEnums.h"
#include "IFieldDefinition.h"
#include "IValue.h"

class IParameter
{
public:
    virtual void setField(IFieldDefinitionPtr field) = 0;
    virtual IFieldDefinitionPtr field() = 0;
    virtual void addValue(IValuePtr value) = 0;
    virtual QList<IValuePtr> values() = 0;
    virtual void setQueryType(VALIDQUERY queryType) = 0;
    virtual VALIDQUERY queryType() = 0;
    virtual ~IParameter() {}
};



#endif // IPARAMETER_H
