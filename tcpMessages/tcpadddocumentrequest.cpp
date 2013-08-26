#include "tcpAddDocumentRequest.h"

TCPAddDocumentRequest::TCPAddDocumentRequest(QObject *parent) :
    MessageBase(ADD_DOCUMENT, parent)
{
}

TCPAddDocumentRequest::~TCPAddDocumentRequest()
{
}

QDataStream& operator<<( QDataStream& dataStream, const TCPAddDocumentRequest& message)
{
    dataStream << message.messageType
               << message.messageSize
               << message.m_Blob;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>( QDataStream& dataStream, TCPAddDocumentRequest& message) // deprecated: throw( UserException )
{
    int aux = 0;
    dataStream >> aux;
    dataStream >> message.messageSize
                  >> message.m_Blob;
    return dataStream;
}
