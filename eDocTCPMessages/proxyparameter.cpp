#include "proxyparameter.h"

ProxyParameter::ProxyParameter(QObject *parent) :
    QObject(parent)
{
    _fieldDefinition = NULL;
    _values.clear();
}

ProxyParameter::~ProxyParameter()
{
}

void ProxyParameter::setField(IFieldDefinition *field)
{
    _fieldDefinition = field;
}

IFieldDefinition* ProxyParameter::field()
{
    return _fieldDefinition;
}

void ProxyParameter::addValue(IValue *value)
{
    _values.push_back(value);
}

QList<IValue*> ProxyParameter::values()
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
