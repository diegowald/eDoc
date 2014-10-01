#ifndef JSONHELPERS_H
#define JSONHELPERS_H

#include <QObject>
#include <QJsonObject>

#include "../eDoc-API/forward.h"
#include "../eDoc-MetadataFramework/valuedefinitions.h"

class JsonHelpers
{
public:
    static QJsonObject toJson(IParameterPtr obj);
    static QJsonObject toJson(IRecordPtr obj);
    static QJsonObject toJson(IRecordIDPtr obj);
    static QJsonObject toJson(QSharedPointer<IDocumentValue> obj);
    static QJsonObject toJson(IValuePtr obj);
    static QJsonObject toJson(IFieldDefinitionPtr obj);
    static QJsonObject toJson(IDocIDPtr obj);
};

#endif // JSONHELPERS_H
