#ifndef HISOTRICRECORD_H
#define HISOTRICRECORD_H

#include "historicdatabaseplugin_global.h"
#include <QObject>
#include "../eDoc-API/IRecord.h"

class HISTORICDATABASEPLUGINSHARED_EXPORT HistoricRecord : public QObject, public IRecord
{
    Q_OBJECT
public:
    explicit HistoricRecord(IRecord* realRecord, QObject *parent = 0);
    virtual ~HistoricRecord();

    virtual void setID(IRecordID *ID);
    virtual IRecordID *ID();
    virtual IValue* value(IFieldDefinition* field);
    virtual IValue* value(const QString &fieldName);
    virtual IFieldDefinition* fieldDefinition(const QString &fieldName);
    virtual QList<QString> fieldNames();

    IRecord *getRecord();
signals:

public slots:

private:
    IRecord *_record;
    IRecordID *_masterID;
};

#endif // HISOTRICRECORD_H
