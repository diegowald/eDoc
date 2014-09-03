#ifndef HISOTRICRECORD_H
#define HISOTRICRECORD_H

#include "historicdatabaseplugin_global.h"
#include <QObject>
#include "../eDoc-API/IRecord.h"

class HISTORICDATABASEPLUGINSHARED_EXPORT HistoricRecord : public QObject, public IRecord
{
    Q_OBJECT
public:
    explicit HistoricRecord(QSharedPointer<IRecord> realRecord, QObject *parent = 0);
    virtual ~HistoricRecord();

    virtual void setID(QSharedPointer<IRecordID> ID);
    virtual QSharedPointer<IRecordID> ID();
    virtual QSharedPointer<IValue> value(QSharedPointer<IFieldDefinition> field);
    virtual QSharedPointer<IValue> value(const QString &fieldName);
    virtual QSharedPointer<IFieldDefinition> fieldDefinition(const QString &fieldName);
    virtual QList<QString> fieldNames();

    QSharedPointer<IRecord> getRecord();
signals:

public slots:

private:
    QSharedPointer<IRecord> _record;
    QSharedPointer<IRecordID> _masterID;
};

typedef QSharedPointer<HistoricRecord> HistoricRecordPtr;
#endif // HISOTRICRECORD_H
