#ifndef PROXYRECORD_H
#define PROXYRECORD_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/IRecord.h"

CONSTRUCT(ProxyRecord)

class EDOCTCPMESSAGESSHARED_EXPORT ProxyRecord : public QObject, public IRecord
{
    Q_OBJECT
public:
    explicit ProxyRecord(QObject *parent = 0);
    virtual ~ProxyRecord();

    virtual void setID(QSharedPointer<IRecordID> ID);
    virtual QSharedPointer<IRecordID> ID();
    virtual QSharedPointer<IValue> value(QSharedPointer<IFieldDefinition> field);
    virtual QSharedPointer<IValue> value(const QString &fieldName);
    virtual QSharedPointer<IFieldDefinition> fieldDefinition(const QString &fieldName);
    virtual QList<QString> fieldNames();

    friend QDataStream& operator>>(QDataStream& is, ProxyRecordPtr obj);

public slots:
    // slots
    virtual void prepareToSave();
    virtual void prepareToLoad();

private:
    QSharedPointer<IRecordID> _ID;
    QMap<QString, IValuePtr> _values;
    QMap<QString, IFieldDefinitionPtr> _fieldDefinitions;
};

#endif // PROXYRECORD_H
