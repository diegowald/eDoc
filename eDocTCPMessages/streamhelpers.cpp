#include "streamhelpers.h"

QDataStream& operator<<(QDataStream& os, IParameter &obj)
{
    os << (int)(obj.field() ? 1 : 0);
    if (obj.field())
    {
        os << *obj.field();
    }
    os << obj.values().count();
    foreach (IValue* value, obj.values())
    {
        os << *value;
    }
    os << obj.queryType();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyParameter &obj)
{
    int isFieldNull = 0;
    is >> isFieldNull;
    if (isFieldNull == 1)
    {    ProxyFieldDefinition *fieldDef = new ProxyFieldDefinition(&obj);
        is >> *fieldDef;
        obj.setField(fieldDef);
    }
    else
    {
        obj.setField(NULL);
    }
    int count = 0;
    is >> count;
    for (int i = 0; i < count; ++i)
    {
        ProxyValue *value = new ProxyValue();
        is >> *value;
        obj._values.push_back(value);
    }
    int queryType;
    is >> queryType;
    obj.setQueryType((VALIDQUERY)queryType);
    return is;
}

QDataStream& operator<<(QDataStream& os, IRecord &obj)
{
    os << *obj.ID();

    os << obj.fieldNames().count();
    foreach (QString field, obj.fieldNames())
    {
        os << *obj.fieldDefinition(field);
        os << *obj.value(field);
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyRecord &obj)
{
    ProxyRecordID *id = new ProxyRecordID();
    is >> *id;
    obj.setID(id);

    int fieldCount;
    is >> fieldCount;

    for (int i = 0; i < fieldCount; ++i)
    {
        ProxyFieldDefinition *fieldDef = new ProxyFieldDefinition();
        is >> *fieldDef;
        ProxyValue *value = new ProxyValue();
        is >> *value;
        obj._fieldDefinitions[fieldDef->name()] = fieldDef;
        obj._values[fieldDef->name()] = value;
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IRecordID &obj)
{
    os << obj.asString();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyRecordID &obj)
{
    QString id;
    is >> id;
    obj.setValue(id);
    return is;
}

QDataStream& operator<<(QDataStream& os, IValue &obj)
{
    os << obj.isNull();
    if (!obj.isNull())
    {
        os << obj.content();
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyValue &obj)
{
    bool isNull;
    is >> isNull;
    if (isNull)
    {
        obj.setNull();
    }
    else
    {
        QVariant value;
        is >> value;
        obj.setValue(value);
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IFieldDefinition &obj)
{
    os << obj.name();
    os << obj.type();
    os << obj.isReadOnly();
    os << obj.isVisible();
    os << obj.isQueryable();

    int count = obj.validQueries().count();
    os << count;
    if (count > 0)
    {
        foreach (VALIDQUERY query, obj.validQueries())
        {
            os << query;
        }
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyFieldDefinition &obj)
{
    QString s;
    is >> s;
    obj.setName(s);
    is >> s;
    obj.setType(s);
    bool v;
    is >> v;
    obj.setIsReadOnly(v);
    is >> v;
    obj.setIsVisible(v);
    is >> v;
    obj.setIsQueryable(v);

    int count;
    is >> count;
    for (int i = 0; i < count; ++i)
    {
        int query;
        is >> query;
        obj.addValidQuery((VALIDQUERY)query);
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IDocID &obj)
{
    os << obj.asString();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyDocID &obj)
{
    is >> obj.id;
    return is;
}
