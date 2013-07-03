#ifndef IFIELDDEFINITION_H
#define IFIELDDEFINITION_H

#include <QString>
#include "APIEnums.h"
#include "../eDoc-Configuration/IXMLContent.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include <QList>

struct IFieldDefinition {
public:
    virtual void initialize(IXMLContent *configuration, QObjectLogging *logger) = 0;
    virtual QString name() = 0;
    virtual QString type() = 0;
    virtual bool isReadOnly() = 0;
    virtual bool isVisible() = 0;
    virtual QList<VALIDQUERY> validQueries() = 0;
    virtual ~IFieldDefinition() {}
};

#endif // IFIELDDEFINITION_H
