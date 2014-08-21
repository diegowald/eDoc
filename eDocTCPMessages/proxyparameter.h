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

    virtual void setField(IFieldDefinition *field);
    virtual IFieldDefinition* field();
    virtual void addValue(IValue *value);
    virtual QList<IValue*> values();
    virtual void setQueryType(VALIDQUERY queryType);
    virtual VALIDQUERY queryType();

    friend QDataStream& operator>>(QDataStream& is, ProxyFieldDefinition &obj);
signals:

public slots:

private:
    IFieldDefinition *_fieldDefinition;
    QList<IValue*> _values;
    VALIDQUERY _queryType;
};

#endif // PROXYPARAMETER_H
