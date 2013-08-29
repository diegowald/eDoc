#ifndef GETBLOBRESPONSE_H
#define GETBLOBRESPONSE_H

#include "messageBase.h"
#include "tcpmessages_global.h"
#include "../eDoc-API/IDocID.h"


class TCPMESSAGESSHARED_EXPORT getBlobResponse : public MessageBase
{
    Q_OBJECT
public:
    explicit getBlobResponse(QObject *parent = 0);
    
signals:
    
public slots:
public:
    QByteArray blob;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& dataStream, const getBlobResponse& msg);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& dataStream, getBlobResponse& msg); // deprecated: throw( UserException )

#endif // GETBLOBRESPONSE_H
