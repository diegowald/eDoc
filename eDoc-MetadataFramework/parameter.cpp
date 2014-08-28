#include "parameter.h"

Parameter::Parameter(QObject *parent) : QObject(parent)
{
}

Parameter::~Parameter()
{
}

void Parameter::setField(QSharedPointer<IFieldDefinition> field)
{
    m_Field = field;
}

QSharedPointer<IFieldDefinition> Parameter::field()
{
    return m_Field;
}

void Parameter::addValue(QSharedPointer<IValue> value)
{
    m_Values.append(value);
}

QList<QSharedPointer<IValue> > Parameter::values()
{
    return m_Values;
}

void Parameter::setQueryType(VALIDQUERY queryType)
{
    m_QueryType = queryType;
}

VALIDQUERY Parameter::queryType()
{
    return m_QueryType;
}

