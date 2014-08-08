#ifndef IFIELDDEFINITION_H
#define IFIELDDEFINITION_H

#include <QString>
#include "APIEnums.h"
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include <QList>
#include "IValue.h"


struct IFieldDefinition {
public:
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger) = 0;
    virtual QString name() = 0;
    virtual QString type() = 0;
    virtual bool isReadOnly() const = 0;
    virtual bool isVisible() const = 0;
    virtual bool isQueryable() const = 0;
    virtual QList<VALIDQUERY> validQueries() = 0;
    virtual IValue* createEmptyValue() = 0;
    virtual ~IFieldDefinition() {}
};

#endif // IFIELDDEFINITION_H
