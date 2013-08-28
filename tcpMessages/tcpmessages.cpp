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
    case ADD_DOCUMENT_REQ:
    {
        AddDocumentRequest req;
        dataStream >> req;
        emit addDocumentRequestArrived(req);
        // y asi con todos
        break;
    }
    default:
        emit badMessage();
        break;
    }
}
