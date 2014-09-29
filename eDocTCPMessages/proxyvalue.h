#ifndef PROXYVALUE_H
#define PROXYVALUE_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/IValue.h"

CONSTRUCT(ProxyValue)

class EDOCTCPMESSAGESSHARED_EXPORT ProxyValue : public QObject, public IValue
{
    Q_OBJECT
public:
    explicit ProxyValue(QObject *parent = 0);
    virtual ~ProxyValue();

    virtual void setValue(const QVariant &newValue);
    virtual QVariant asVariant();
    virtual QVariant content();
    virtual void setNull();
    virtual bool isNull();

    // slots
public slots:
    virtual void prepareToSave() {}
    virtual void prepareToLoad() {}

signals:

public slots:

private:
    QVariant _value;
    bool _isNull;
};

#endif // PROXYVALUE_H
