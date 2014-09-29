#include "proxyfielddefinition.h"
#include "proxyvalue.h"
#include <QJsonArray>

ProxyFieldDefinition::ProxyFieldDefinition(QObject *parent) :
    QObject(parent)
{
    _name = "";
    _type = "";
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

QString ProxyFieldDefinition::type()
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

void ProxyFieldDefinition::setType(const QString &newType)
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

QJsonObject ProxyFieldDefinition::asJson()
{
    QJsonObject obj;
    obj["name"] = name();
    obj["type"] = type();
    obj["isReadOnly"] = isReadOnly();
    obj["isVisible"] = isVisible();
    obj["isQueryable"] = isQueryable();

    obj["count"] = validQueries().count();
    QJsonArray arr;
    foreach (VALIDQUERY query, validQueries())
    {
        QJsonObject o;
        o["query"] = query;
        arr.append(o);
    }
    obj["validQueries"] = arr;
    return obj;
}
