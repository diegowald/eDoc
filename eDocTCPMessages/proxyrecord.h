#ifndef PROXYRECORD_H
#define PROXYRECORD_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/IRecord.h"

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

    friend QDataStream& operator>>(QDataStream& is, ProxyRecord &obj);
signals:

public slots:

private:
    QSharedPointer<IRecordID> _ID;
    QMap<QString, QSharedPointer<IValue>> _values;
    QMap<QString, QSharedPointer<IFieldDefinition>> _fieldDefinitions;
};

#endif // PROXYRECORD_H
