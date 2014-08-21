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

    virtual void setID(IRecordID *ID);
    virtual IRecordID *ID();
    virtual IValue* value(IFieldDefinition* field);
    virtual IValue* value(const QString &fieldName);
    virtual IFieldDefinition* fieldDefinition(const QString &fieldName);
    virtual QList<QString> fieldNames();

    friend QDataStream& operator>>(QDataStream& is, ProxyRecord &obj);
signals:

public slots:

private:
    IRecordID *_ID;
    QMap<QString, IValue*> _values;
    QMap<QString, IFieldDefinition*> _fieldDefinitions;
};

#endif // PROXYRECORD_H
