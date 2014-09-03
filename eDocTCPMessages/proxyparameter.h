#ifndef PROXYPARAMETER_H
#define PROXYPARAMETER_H

#include <QObject>
#include "edoctcpmessages_global.h"

#include "../eDoc-API/IParameter.h"

#include "proxyfielddefinition.h"
#include "proxyvalue.h"

class EDOCTCPMESSAGESSHARED_EXPORT ProxyParameter : public QObject, public IParameter
{
    Q_OBJECT
public:
    explicit ProxyParameter(QObject *parent = 0);
    virtual ~ProxyParameter();

    virtual void setField(QSharedPointer<IFieldDefinition> field);
    virtual QSharedPointer<IFieldDefinition> field();
    virtual void addValue(QSharedPointer<IValue> value);
    virtual QList<QSharedPointer<IValue>> values();
    virtual void setQueryType(VALIDQUERY queryType);
    virtual VALIDQUERY queryType();

    friend QDataStream& operator>>(QDataStream& is, ProxyParameter &obj);
signals:

public slots:

private:
    QSharedPointer<IFieldDefinition> _fieldDefinition;
    QList<QSharedPointer<IValue>> _values;
    VALIDQUERY _queryType;
};

#endif // PROXYPARAMETER_H
