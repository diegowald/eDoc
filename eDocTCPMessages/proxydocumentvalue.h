#ifndef PROXYDOCUMENTVALUE_H
#define PROXYDOCUMENTVALUE_H
#include "proxyvalue.h"

CONSTRUCT(ProxyDocumentValue)

class EDOCTCPMESSAGESSHARED_EXPORT ProxyDocumentValue : public ProxyValue
{
    Q_OBJECT
public:
    explicit ProxyDocumentValue(QObject *parent = 0);
    virtual ~ProxyDocumentValue();

    friend QDataStream& operator>>(QDataStream& is, ProxyDocumentValuePtr obj);

signals:

public slots:
    // slots
    virtual void prepareToSave();
    virtual void prepareToLoad();

private:
    IDocIDPtr _docID;
};

#endif // PROXYDOCUMENTVALUE_H
