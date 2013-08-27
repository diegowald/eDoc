#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include <QObject>
#include "tcpmessages_global.h"
#include <QDataStream>

enum MessageType
{
    ADD_DOCUMENT
};

class TCPMESSAGESSHARED_EXPORT MessageBase : public QObject
{
    Q_OBJECT
public:
    explicit MessageBase(MessageType type, QObject *parent = 0);
    virtual ~MessageBase();

public:
    int messageSize;
    MessageType messageType;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& dataStream, const MessageBase& messageBase);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& dataStream, MessageBase& messageBase); // deprecated: throw( UserException )


#endif // MESSAGEBASE_H
