#ifndef PROXYDOCUMENT_H
#define PROXYDOCUMENT_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/forward.h"
#include "../eDoc-API/IDocBase.h"

CONSTRUCT(ProxyDocument)

class EDOCTCPMESSAGESSHARED_EXPORT ProxyDocument : public QObject, public IDocBase
{
    Q_OBJECT
public:
    explicit ProxyDocument(IDocIDPtr docId, QObject *parent = 0);
    virtual ~ProxyDocument();

    virtual IDocIDPtr id();
    virtual bool isComplex() const;
    virtual long size();
signals:

public slots:

private:
    IDocIDPtr _docId;
};

#endif // PROXYDOCUMENT_H
