#include "proxyfielddefinition.h"
#include "proxyvalue.h"

ProxyFieldDefinition::ProxyFieldDefinition(QObject *parent) :
    QObject(parent)
{
    _name = "";
    _type = DATATYPE::INVALID_TYPE;
    _typeString = "";
    _isReadOnly = false;
    _isVisible = false;
    _isQueryable = false;
    _validQueries.clear();
}

ProxyFieldDefinition::~ProxyFieldDefinition()
{
}

void ProxyFieldDefinition::initialize(IXMLContentPtr configuration, IFactory *factory)
{
    (void)configuration;
    (void)factory;
}

QString ProxyFieldDefinition::name()
{
    return _name;
}

QString ProxyFieldDefinition::typeAsString()
{
    return _typeString;
}

DATATYPE ProxyFieldDefinition::type()
{
    return _type;
}

bool ProxyFieldDefinition::isReadOnly() const
{
    return _isReadOnly;
}

bool ProxyFieldDefinition::isVisible() const
{
    return _isVisible;
}

bool ProxyFieldDefinition::isQueryable() const
{
    return _isQueryable;
}

QList<VALIDQUERY> ProxyFieldDefinition::validQueries()
{
    return _validQueries;
}

QSharedPointer<IValue> ProxyFieldDefinition::createEmptyValue()
{
    return QSharedPointer<IValue>(new ProxyValue());
}

void ProxyFieldDefinition::setName(const QString &newName)
{
    _name = newName;
}

void ProxyFieldDefinition::setTypeString(const QString &newType)
{
    _typeString = newType;
}

void ProxyFieldDefinition::setType(DATATYPE newType)
{
    _type = newType;
}

void ProxyFieldDefinition::setIsReadOnly(bool value)
{
    _isReadOnly = value;
}

void ProxyFieldDefinition::setIsVisible(bool value)
{
    _isVisible = value;
}

void ProxyFieldDefinition::setIsQueryable(bool value)
{
    _isQueryable = value;
}

void ProxyFieldDefinition::addValidQuery(VALIDQUERY query)
{
    _validQueries.push_back(query);
}
