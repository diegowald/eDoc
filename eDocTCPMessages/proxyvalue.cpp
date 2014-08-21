#include "proxyvalue.h"
#include <QVariant>

ProxyValue::ProxyValue(QObject *parent) :
    QObject(parent)
{
    _value.clear();
    _isNull = true;
}

ProxyValue::~ProxyValue()
{
}

void ProxyValue::setValue(const QVariant &newValue)
{
    if (!newValue.isNull())
    {
        _value = newValue;
        _isNull = false;
    }
    else
    {
        setNull();
    }
}

QVariant ProxyValue::asVariant()
{
    return _value;
}

QVariant ProxyValue::content()
{
    return _value;
}

void ProxyValue::setNull()
{
    _value.clear();
    _isNull = true;
}

bool ProxyValue::isNull()
{
    return _isNull;
}
