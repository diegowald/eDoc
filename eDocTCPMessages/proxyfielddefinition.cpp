#include "proxyfielddefinition.h"
#include "proxyvalue.h"

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

void ProxyFieldDefinition::initialize(QSharedPointer<IXMLContent> configuration,
                                      QSharedPointer<QObjectLogging> logger,
                                      const QMap<QString, QString> &docpluginStock,
                                      const QMap<QString, QString> &DBplugins,
                                      const QMap<QString, QString> &DBWithHistoryPlugins,
                                      const QMap<QString, QString> &tagPlugins,
                                      const QMap<QString, QString> &serverPlugins)
{
    (void)configuration;
    (void)logger;
    (void)docpluginStock;
    (void)DBplugins;
    (void)DBWithHistoryPlugins;
    (void)tagPlugins;
    (void)serverPlugins;
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
    return QSharedPointer<IValue>(new ProxyValue(this));
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
