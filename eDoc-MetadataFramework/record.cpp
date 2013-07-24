#include "record.h"

Record::Record(const QList<IFieldDefinition *> &fieldDefs, QObject *parent) :
    QObject(parent)
{
    foreach (IFieldDefinition* fDef, fieldDefs) {
        m_Values[fDef->name()] = fDef->createEmptyValue();
        m_Fields[fDef->name()] = fDef;
    }
}

Record::~Record()
{
}

void Record::setID(IRecordID *ID)
{
    m_ID = ID;
}

IRecordID *Record::ID()
{
    return m_ID;
}

IValue* Record::value(IFieldDefinition* field)
{
    return value(field->name());
}

IValue* Record::value(const QString &fieldName)
{
    if (m_Values.contains(fieldName))
        return m_Values[fieldName];
    else
        return NULL; // ACA deberia lanzar una excepcion
}

IFieldDefinition* Record::fieldDefinition(const QString &fieldName)
{
    if (m_Fields.contains(fieldName))
        return m_Fields[fieldName];
    else
        return NULL; // ACA deberia lanzar una excepcion
}

QList<QString> Record::fieldNames()
{
    return m_Fields.keys();
}
