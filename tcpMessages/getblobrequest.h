#ifndef GETBLOBREQUEST_H
#define GETBLOBREQUEST_H

#include <QObject>
#include "messageBase.h"
#include "tcpmessages_global.h"
#include "../eDoc-API/IDocID.h"

class TCPMESSAGESSHARED_EXPORT getBlobRequest  : public MessageBase
{
    Q_OBJECT
public:
    explicit getBlobRequest(IDocID *Id, QObject *parent = 0);
signals:
    
public slots:
public:
    IDocID *id;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& dataStream, const getBlobRequest& msg);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& dataStream, getBlobRequest& msg); // deprecated: throw( UserException )


#endif // GETBLOBREQUEST_H
