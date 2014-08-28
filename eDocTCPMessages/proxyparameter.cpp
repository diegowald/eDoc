#include "proxyparameter.h"

ProxyParameter::ProxyParameter(QObject *parent) :
    QObject(parent)
{
    _values.clear();
}

ProxyParameter::~ProxyParameter()
{
}

void ProxyParameter::setField(QSharedPointer<IFieldDefinition> field)
{
    _fieldDefinition = field;
}

QSharedPointer<IFieldDefinition> ProxyParameter::field()
{
    return _fieldDefinition;
}

void ProxyParameter::addValue(QSharedPointer<IValue> value)
{
    _values.push_back(value);
}

QList<QSharedPointer<IValue> > ProxyParameter::values()
{
    return _values;
}

void ProxyParameter::setQueryType(VALIDQUERY queryType)
{
    _queryType = queryType;
}

VALIDQUERY ProxyParameter::queryType()
{
    return _queryType;
}
