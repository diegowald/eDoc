#include "parameter.h"

Parameter::Parameter(QObject *parent) : QObject(parent)
{
    m_Field = NULL;
}

Parameter::~Parameter()
{
}

void Parameter::setField(IFieldDefinition *field)
{
    m_Field = field;
}

IFieldDefinition* Parameter::field()
{
    return m_Field;
}

void Parameter::addValue(IValue *value)
{
    m_Values.append(value);
}

QList<IValue*> Parameter::values()
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

