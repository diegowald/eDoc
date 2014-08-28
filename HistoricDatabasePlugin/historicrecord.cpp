#include "historicrecord.h"

HistoricRecord::HistoricRecord(QSharedPointer<IRecord> realRecord, QObject *parent) :
    QObject(parent)
{
    _record = realRecord;
}

HistoricRecord::~HistoricRecord()
{
}

void HistoricRecord::setID(QSharedPointer<IRecordID> ID)
{
    _masterID = ID;
}

QSharedPointer<IRecordID> HistoricRecord::ID()
{
    return _masterID;
}

QSharedPointer<IValue> HistoricRecord::value(QSharedPointer<IFieldDefinition> field)
{
    return _record->value(field);
}

QSharedPointer<IValue> HistoricRecord::value(const QString &fieldName)
{
    return _record->value(fieldName);
}

QSharedPointer<IFieldDefinition> HistoricRecord::fieldDefinition(const QString &fieldName)
{
    return _record->fieldDefinition(fieldName);
}

QList<QString> HistoricRecord::fieldNames()
{
    return _record->fieldNames();
}

QSharedPointer<IRecord> HistoricRecord::getRecord()
{
    return _record;
}
