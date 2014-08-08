#include "tcpmessages.h"


TcpMessages::TcpMessages()
{
}


void TcpMessages::parse(QDataStream &dataStream)
{
    MessageBase msg;
    dataStream >> msg;
    dataStream.device()->seek(0);
    switch (msg.messageType)
    {
    case INVALID_MESSAGE:
    {
        InvalidMessage req;
        dataStream >> req;
        emit InvalidMessageArrived(req);
        break;
    }
    case ADD_DOCUMENT_REQ:
    {
        AddDocumentRequest req;
        dataStream >> req;
        emit addDocumentRequestArrived(req);
        // y asi con todos
        break;
    }
        ADD_DOCUMENT_RESP,

    case TCP_CHUNK_SEND:
    {
        TCPChunck req;
        dataStream >> req;
        emit ChunkArrived(req);
        break;
    }
        TCP_CHUNK_RESP,
        GET_BLOB_REQ,
        GET_BLOB_RESP
    default:
        emit badMessage();
        break;
    }
}
