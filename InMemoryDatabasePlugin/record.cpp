#include "record.h"
#include "fielddefinition.h"

Record::Record(const QList<IFieldDefinition*> &fieldDefs, QObject *parent) :
    QObject(parent)
{
    m_ID = new RecordID(this);
    foreach (IFieldDefinition* fDef, fieldDefs) {
        FieldDefinition *fd = (FieldDefinition*) fDef;
        m_Values[fd->name()] = fd->createEmptyValue();
    }
}

Record::~Record()
{
}

void Record::setID(IRecordID *ID)
{
    m_ID = (RecordID*) ID;
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
