#ifndef TCPCHUNCK_H
#define TCPCHUNCK_H

#include "messageBase.h"

class TCPMESSAGESSHARED_EXPORT TCPChunck : public MessageBase
{
public:
    TCPChunck(QObject *parent = 0);

public:
    QByteArray m_Blob;
    int m_part;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<( QDataStream& dataStream, const TCPChunck& message);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>( QDataStream& dataStream, TCPChunck& message); // deprecated: throw( UserException )


#endif // TCPCHUNCK_H
