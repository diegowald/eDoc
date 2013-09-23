#include "tcpchunck.h"

TCPChunck::TCPChunck(QObject *parent) :
    MessageBase(TCP_CHUNK_SEND, parent)
{
}


QDataStream& operator<<(QDataStream& dataStream, const TCPChunck& message)
{
    dataStream << (*(MessageBase*)(&message))
                  << message.m_part
                  << message.m_Blob;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, TCPChunck& message) // deprecated: throw( UserException )
{
    int aux = 0;
    dataStream >> (*(MessageBase*)(&message))
               >> message.m_part
               >> message.m_Blob;
    message.messageType = (MessageType) aux;
    return dataStream;
}
