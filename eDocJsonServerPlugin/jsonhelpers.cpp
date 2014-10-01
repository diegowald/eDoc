#include "jsonhelpers.h"
#include <QJsonArray>
#include "../eDoc-API/IParameter.h"

QJsonObject JsonHelpers::toJson(IParameterPtr obj)
{
    QJsonObject response;
    response["isnull"] = (int)(obj->field() ? 1 : 0);
    if (obj->field())
    {
        response["field"] = JsonHelpers::toJson(obj->field());
    }

    response["valuesCount"] = obj->values().count();
    QJsonArray vals;
    foreach (IValuePtr value, obj->values())
    {
        vals.append(JsonHelpers::toJson(value));
    }
    response["values"] = vals;
    response["queryType"] = obj->queryType();
    return response;
}

QJsonObject JsonHelpers::toJson(IRecordPtr obj)
{
    QJsonObject response;
    response["id"] = JsonHelpers::toJson(obj->ID());

    response["fieldsCount"] = obj->fieldNames().count();
    foreach (QString field, obj->fieldNames())
    {
        response["fieldDefinition"] = JsonHelpers::toJson(obj->fieldDefinition(field));
        if (obj->fieldDefinition(field)->type() == "document")
        {
            response["field"] = JsonHelpers::toJson(obj->value(field).dynamicCast<IDocumentValue>());
        }
        else
        {
            response["field"] = JsonHelpers::toJson(obj->value(field));
        }
    }
    return response;
}

QJsonObject JsonHelpers::toJson(IRecordIDPtr obj)
{
    QJsonObject response;
    response["id"] = obj->asString();
    return response;
}

QJsonObject JsonHelpers::toJson(IValuePtr obj)
{
    QJsonObject response;

    response["isnull"] = obj->isNull();
    if (!obj->isNull())
    {
        response["content"] = obj->content().toJsonObject();
    }
    return response;
}


QJsonObject JsonHelpers::toJson(QSharedPointer<IDocumentValue> obj)
{
    QJsonObject response;
    response["isnull"] = obj->isNull();
    if (!obj->isNull())
    {
        response["content"] = JsonHelpers::toJson(qvariant_cast<IDocumentPtr>(obj->content())->id());
    }
    return response;
}

QJsonObject JsonHelpers::toJson(IFieldDefinitionPtr obj)
{
    QJsonObject response;
    response["name"] = obj->name();
    response["type"] = obj->type();
    response["isreadOnly"] = obj->isReadOnly();
    response["isVisible"] = obj->isVisible();
    response["isQUeryable"] = obj->isQueryable();

    int count = obj->validQueries().count();
    response["validQueryCount"] = count;
    QJsonArray queries;
    if (count > 0)
    {
        foreach (VALIDQUERY query, obj->validQueries())
        {
            QJsonObject qry;
            qry["query"] = query;
            queries.append(qry);
        }
    }
    response["validqueries"] = queries;
    return response;
}

QJsonObject JsonHelpers::toJson(IDocIDPtr obj)
{
    QJsonObject response;
    response["id"] = obj->asString();
    return response;
}

