#include "streamhelpers.h"

QDataStream& operator<<(QDataStream& os, IParameterPtr obj)
{
    os << (int)(obj->field() ? 1 : 0);
    if (obj->field())
    {
        os << obj->field();
    }
    os << obj->values().count();
    foreach (IValuePtr value, obj->values())
    {
        os << value;
    }
    os << obj->queryType();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyParameterPtr obj)
{
    int isFieldNull = 0;
    is >> isFieldNull;
    if (isFieldNull == 1)
    {
        ProxyFieldDefinitionPtr fieldDef = ProxyFieldDefinitionPtr(new ProxyFieldDefinition());
        is >> fieldDef;
        obj->setField(fieldDef);
    }
    else
    {
        obj->setField(IFieldDefinitionPtr());
    }
    int count = 0;
    is >> count;
    for (int i = 0; i < count; ++i)
    {
        ProxyValuePtr value = ProxyValuePtr(new ProxyValue());
        is >> value;
        obj->addValue(value);
    }
    int queryType;
    is >> queryType;
    obj->setQueryType((VALIDQUERY)queryType);
    return is;
}

QDataStream& operator<<(QDataStream& os, IRecordPtr obj)
{
    os << obj->ID();

    os << obj->fieldNames().count();
    foreach (QString field, obj->fieldNames())
    {
        os << obj->fieldDefinition(field);
        if (obj->fieldDefinition(field)->type() == "document")
        {
            os << obj->value(field).dynamicCast<IDocumentValue>();
        }
        else
        {
            os << obj->value(field);
        }
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyRecordPtr obj)
{
    ProxyRecordIDPtr id = ProxyRecordIDPtr(new ProxyRecordID());
    is >> id;
    obj->setID(id);

    int fieldCount;
    is >> fieldCount;

    for (int i = 0; i < fieldCount; ++i)
    {
        ProxyFieldDefinitionPtr fieldDef = ProxyFieldDefinitionPtr(new ProxyFieldDefinition());
        is >> fieldDef;

        QSharedPointer<ProxyValue> value;
        if (fieldDef->type() == "document")
        {
            ProxyDocumentValuePtr v = ProxyDocumentValuePtr(new ProxyDocumentValue());
            is >> v;
            value = v;
        }
        else
        {
            value = ProxyValuePtr(new ProxyValue());
            is >> value;
        }
        obj->_fieldDefinitions[fieldDef->name()] = fieldDef;
        obj->_values[fieldDef->name()] = value;
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IRecordIDPtr obj)
{
    os << obj->asString();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyRecordIDPtr obj)
{
    QString id;
    is >> id;
    obj->setValue(id);
    return is;
}

QDataStream& operator<<(QDataStream& os, IValuePtr obj)
{
    os << obj->isNull();
    if (!obj->isNull())
    {
        os << obj->content();
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyValuePtr obj)
{
    bool isNull;
    is >> isNull;
    if (isNull)
    {
        obj->setNull();
    }
    else
    {
        QVariant value;
        is >> value;
        obj->setValue(value);
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, QSharedPointer<IDocumentValue> obj)
{
    os << obj->isNull();
    if (!obj->isNull())
    {
        os << qvariant_cast<IDocumentPtr>(obj->content())->id()->asString();
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyDocumentValuePtr obj)
{
    bool isNull;
    is >> isNull;
    if (isNull)
    {
        obj->setNull();
    }
    else
    {
        ProxyDocIDPtr docID = ProxyDocIDPtr(new ProxyDocID());
        is >> docID;
        obj->_docID = docID;
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IFieldDefinitionPtr obj)
{
    os << obj->name();
    os << obj->type();
    os << obj->isReadOnly();
    os << obj->isVisible();
    os << obj->isQueryable();

    int count = obj->validQueries().count();
    os << count;
    if (count > 0)
    {
        foreach (VALIDQUERY query, obj->validQueries())
        {
            os << query;
        }
    }
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyFieldDefinitionPtr obj)
{
    QString s;
    is >> s;
    obj->setName(s);
    is >> s;
    obj->setType(s);
    bool v;
    is >> v;
    obj->setIsReadOnly(v);
    is >> v;
    obj->setIsVisible(v);
    is >> v;
    obj->setIsQueryable(v);

    int count;
    is >> count;
    for (int i = 0; i < count; ++i)
    {
        int query;
        is >> query;
        obj->addValidQuery((VALIDQUERY)query);
    }
    return is;
}

QDataStream& operator<<(QDataStream& os, IDocIDPtr obj)
{
    os << obj->asString();
    return os;
}

QDataStream& operator>>(QDataStream& is, ProxyDocIDPtr obj)
{
    is >> obj->id;
    return is;
}
