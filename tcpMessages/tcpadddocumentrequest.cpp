#include "tcpAddDocumentRequest.h"

AddDocumentRequest::AddDocumentRequest(QObject *parent) :
    MessageBase(ADD_DOCUMENT_REQ, parent)
{
}

AddDocumentRequest::~AddDocumentRequest()
{
}

void AddDocumentRequest::setBlob(const QByteArray &blob)
{
    m_Blob = blob;
}

QDataStream& operator<<(QDataStream& dataStream, const AddDocumentRequest& message)
{
    dataStream << (*(MessageBase*)(&message))
    /*dataStream << message.messageType
               << message.messageSize*/
               << message.m_Blob;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, AddDocumentRequest& message) // deprecated: throw( UserException )
{
    int aux = 0;
    dataStream >> (*(MessageBase*)(&message))
               >> message.m_Blob;
    message.messageType = (MessageType) aux;
    return dataStream;
}
