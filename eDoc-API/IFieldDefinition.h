#ifndef IFIELDDEFINITION_H
#define IFIELDDEFINITION_H

#include <QString>
#include "APIEnums.h"


struct IFieldDefinition {
public:
    QString name() = 0;
    QString type() = 0;
    bool isReadOnly() = 0;
    bool isVisible() = 0;
    QList<VALIDQUERY> validQueries();
    virtual ~IFieldDefinition() {}
};

#endif // IFIELDDEFINITION_H
