#include "adddocumentresponse.h"

AddDocumentResponse::AddDocumentResponse(QObject *parent) :
    MessageBase(ADD_DOCUMENT_RESP, parent)
{
}


QDataStream& operator<<(QDataStream& dataStream, const AddDocumentResponse& msg)
{
    dataStream << (*(MessageBase*)(&msg))
               << msg.m_DocID;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, AddDocumentResponse& msg) // deprecated: throw( UserException
{
    dataStream >> (*(MessageBase*)(&msg))
               >> msg.m_DocID;
    return dataStream;
}
