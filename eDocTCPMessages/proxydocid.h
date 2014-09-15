#ifndef PROXYDOCID_H
#define PROXYDOCID_H

#include "edoctcpmessages_global.h"
#include <QObject>
#include "../eDoc-API/forward.h"
#include "../eDoc-API/IDocID.h"

CONSTRUCT(ProxyDocID)


class EDOCTCPMESSAGESSHARED_EXPORT ProxyDocID : public QObject, public IDocID
{
    Q_OBJECT
public:
    explicit ProxyDocID(QObject *parent = 0);
    virtual ~ProxyDocID();
    virtual QString asString() const;

    friend QDataStream& operator>>(QDataStream& is, ProxyDocID &obj);
signals:

public slots:

private:
    QString id;
};

#endif // PROXYDOCID_H
