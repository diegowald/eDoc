#ifndef IFIELDDEFINITION_H
#define IFIELDDEFINITION_H

#include <QString>
#include "forward.h"
#include "APIEnums.h"
#include "IInitializable.h"
#include "iJsonSerializable.h"
#include <QList>
#include "IValue.h"

struct IFieldDefinition : public IInitializable, public IJsonSerializable
{
public:
    virtual QString name() = 0;
    virtual QString type() = 0;
    virtual bool isReadOnly() const = 0;
    virtual bool isVisible() const = 0;
    virtual bool isQueryable() const = 0;
    virtual QList<VALIDQUERY> validQueries() = 0;
    virtual IValuePtr createEmptyValue() = 0;
    virtual ~IFieldDefinition() {}
};

#endif // IFIELDDEFINITION_H
