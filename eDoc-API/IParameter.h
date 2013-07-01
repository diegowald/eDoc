#ifndef IPARAMETER_H
#define IPARAMETER_H

#include <APIEnums.h>
#include <IFieldDefinition.h>

struct IParameter
{
public:
    IFieldDefinition* field() = 0;

    virtual ~IParameter() {}
};

#endif // IPARAMETER_H
