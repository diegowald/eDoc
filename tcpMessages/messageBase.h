#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include <QObject>
#include "tcpmessages_global.h"
#include <QDataStream>

enum MessageType
{
    INVALID_MESSAGE,
    ADD_DOCUMENT_REQ,
    ADD_DOCUMENT_RESP,
    GET_BLOB_REQ,
    GET_BLOB_RESP
};

class TCPMESSAGESSHARED_EXPORT MessageBase : public QObject
{
    Q_OBJECT
public:
    explicit MessageBase(QObject *parent = 0);
    MessageBase(MessageType type, QObject *parent = 0);
    virtual ~MessageBase();

public:
    int messageSize;
    MessageType messageType;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& dataStream, const MessageBase& messageBase);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& dataStream, MessageBase& messageBase); // deprecated: throw( UserException )


#endif // MESSAGEBASE_H
