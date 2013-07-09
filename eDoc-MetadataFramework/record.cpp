#include "record.h"

Record::Record(const QList<IFieldDefinition*> &fieldDefs, QObject *parent) :
    QObject(parent)
{
    foreach (IFieldDefinition* fDef, fieldDefs) {
        m_Values[fDef->name()] = fDef->createEmptyValue();
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
        return NULL; // ACA deberoa lanzar una excepcion
}
