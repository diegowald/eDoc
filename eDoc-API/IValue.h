#ifndef IVALUE_H
#define IVALUE_H
#include <QVariant>
#include "forward.h"

class IValue
{
public:
    virtual void setValue(const QVariant &newValue) = 0;
    virtual QVariant asVariant() = 0;
    virtual QVariant content() = 0;
    virtual void setNull() = 0;
    virtual bool isNull() = 0;

    // slots
    virtual void prepareToSave() = 0;
    virtual void prepareToLoad() = 0;

    virtual ~IValue() {}
};


#endif // IVALUE_H
