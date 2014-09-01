#ifndef PROXYRECORDID_H
#define PROXYRECORDID_H

#include <QObject>
#include "edoctcpmessages_global.h"
#include "../eDoc-API/IRecordID.h"

class EDOCTCPMESSAGESSHARED_EXPORT ProxyRecordID : public QObject, public IRecordID
{
    Q_OBJECT
public:
    explicit ProxyRecordID(QObject *parent = 0);
    virtual ~ProxyRecordID();

    virtual QString asString() const;
    virtual QSharedPointer<IMetadata> metadata();

    void setValue(const QString &value);

signals:

public slots:

private:
    QString _id;
};

typedef QSharedPointer<ProxyRecordID> ProxyRecordIDPtr;

#endif // PROXYRECORDID_H
