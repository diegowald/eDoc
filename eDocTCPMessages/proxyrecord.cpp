#include "proxyrecord.h"

ProxyRecord::ProxyRecord(QObject *parent) :
    QObject(parent)
{
    _ID = NULL;
    _values.clear();
    _fieldDefinitions.clear();
}

ProxyRecord::~ProxyRecord()
{
}

void ProxyRecord::setID(IRecordID *ID)
{
    _ID = ID;
}

IRecordID *ProxyRecord::ID()
{
    return _ID;
}

IValue* ProxyRecord::value(IFieldDefinition* field)
{
    return value(field->name());
}

IValue* ProxyRecord::value(const QString &fieldName)
{
    return _values[fieldName];
}

IFieldDefinition* ProxyRecord::fieldDefinition(const QString &fieldName)
{
    return _fieldDefinitions[fieldName];
}

QList<QString> ProxyRecord::fieldNames()
{
    return _values.keys();
}
