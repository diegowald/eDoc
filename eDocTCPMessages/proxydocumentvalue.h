#ifndef PROXYDOCUMENTVALUE_H
#define PROXYDOCUMENTVALUE_H
#include "proxyvalue.h"
class EDOCTCPMESSAGESSHARED_EXPORT ProxyDocumentValue : public ProxyValue
{
    Q_OBJECT
public:
    explicit ProxyDocumentValue(QObject *parent = 0);
    virtual ~ProxyDocumentValue();

signals:

public slots:
    // slots
    virtual void prepareToSave();
    virtual void prepareToLoad();
};

#endif // PROXYDOCUMENTVALUE_H
