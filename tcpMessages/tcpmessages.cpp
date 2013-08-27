#include "tcpmessages.h"


TcpMessages::TcpMessages()
{
}


void TcpMessages::parse(QDataStream &dataStream)
{
    MessageBase msg;
    dataStream >> msg;
    dataStream.device()->seek(0);
    switch(msg.messageType)
    {
    case ADD_DOCUMENT:
        TCPAddDocumentRequest req;
        dataStream >> req;
        emit addDocumentRequestArrived(req);
        break;
        // y asi con todos
    }
}
