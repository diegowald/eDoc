#ifndef IPARAMETER_H
#define IPARAMETER_H

#include <APIEnums.h>
#include <IFieldDefinition.h>
#include <IValue.h>

struct IParameter
{
public:
    virtual IFieldDefinition* field() = 0;
    virtual QList<IValue*> values() = 0;
    virtual VALIDQUERY queryType() = 0;
    virtual ~IParameter() {}
};

#endif // IPARAMETER_H
