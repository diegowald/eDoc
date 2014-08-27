#include "historicrecord.h"

HistoricRecord::HistoricRecord(IRecord* realRecord, QObject *parent) :
    QObject(parent)
{
    _record = realRecord;
    _masterID = NULL;
}

HistoricRecord::~HistoricRecord()
{
}

void HistoricRecord::setID(IRecordID *ID)
{
    _masterID = ID;
}

IRecordID *HistoricRecord::ID()
{
    return _masterID;
}

IValue* HistoricRecord::value(IFieldDefinition* field)
{
    return _record->value(field);
}

IValue* HistoricRecord::value(const QString &fieldName)
{
    return _record->value(fieldName);
}

IFieldDefinition* HistoricRecord::fieldDefinition(const QString &fieldName)
{
    return _record->fieldDefinition(fieldName);
}

QList<QString> HistoricRecord::fieldNames()
{
    return _record->fieldNames();
}

IRecord* HistoricRecord::getRecord()
{
    return _record;
}
