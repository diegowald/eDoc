#include "proxyrecord.h"

ProxyRecord::ProxyRecord(QObject *parent) :
    QObject(parent)
{
    _values.clear();
    _fieldDefinitions.clear();
}

ProxyRecord::~ProxyRecord()
{
}

void ProxyRecord::setID(QSharedPointer<IRecordID> ID)
{
    _ID = ID;
}

QSharedPointer<IRecordID> ProxyRecord::ID()
{
    return _ID;
}

QSharedPointer<IValue> ProxyRecord::value(QSharedPointer<IFieldDefinition> field)
{
    return value(field->name());
}

QSharedPointer<IValue> ProxyRecord::value(const QString &fieldName)
{
    return _values[fieldName];
}

QSharedPointer<IFieldDefinition> ProxyRecord::fieldDefinition(const QString &fieldName)
{
    return _fieldDefinitions[fieldName];
}

QList<QString> ProxyRecord::fieldNames()
{
    return _values.keys();
}

void ProxyRecord::prepareToLoad()
{
    foreach (IValuePtr value, _values.values())
    {
        value->prepareToLoad();
    }
}

void ProxyRecord::prepareToSave()
{
    foreach (IValuePtr value, _values.values())
    {
        value->prepareToSave();
    }
}
