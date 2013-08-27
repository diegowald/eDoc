#include "messageBase.h"
#include <QIODevice>

MessageBase::MessageBase(MessageType type, QObject *parent) :
    QObject(parent), messageType(type)
{
}

MessageBase::~MessageBase()
{
}

QDataStream& operator<<(QDataStream& dataStream, MessageBase const & messageBase)
{
    dataStream << messageBase.messageType;
    dataStream << messageBase.messageSize;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, MessageBase& messageBase)
{
    int aux = 0;
    dataStream >> aux;
    messageBase.messageType = (MessageType) aux;
    dataStream >> messageBase.messageSize;
    return dataStream;
}
