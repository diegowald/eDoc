#include "record.h"

Record::Record(const QList<QSharedPointer<IFieldDefinition>> &fieldDefs, QObject *parent) :
    QObject(parent)
{
    foreach (QSharedPointer<IFieldDefinition> fDef, fieldDefs)
    {
        m_Values[fDef->name()] = fDef->createEmptyValue();
        m_Fields[fDef->name()] = fDef;
    }
}

Record::~Record()
{
}

void Record::setID(QSharedPointer<IRecordID> ID)
{
    m_ID = ID;
}

QSharedPointer<IRecordID> Record::ID()
{
    return m_ID;
}

QSharedPointer<IValue> Record::value(QSharedPointer<IFieldDefinition> field)
{
    return value(field->name());
}

QSharedPointer<IValue> Record::value(const QString &fieldName)
{
    if (m_Values.contains(fieldName))
        return m_Values[fieldName];
    else
        return QSharedPointer<IValue>(); // ACA deberia lanzar una excepcion
}

QSharedPointer<IFieldDefinition> Record::fieldDefinition(const QString &fieldName)
{
    if (m_Fields.contains(fieldName))
        return m_Fields[fieldName];
    else
        return QSharedPointer<IFieldDefinition>(); // ACA deberia lanzar una excepcion
}

QList<QString> Record::fieldNames()
{
    return m_Fields.keys();
}
