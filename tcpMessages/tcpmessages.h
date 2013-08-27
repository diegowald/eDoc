#ifndef TCPMESSAGES_H
#define TCPMESSAGES_H

#include "tcpmessages_global.h"
#include "messageBase.h"
#include "tcpAddDocumentRequest.h"

class TCPMESSAGESSHARED_EXPORT TcpMessages : public QObject
{
public:
    TcpMessages();
    bool parse();
signals:
    void addDocumentRequestArrived(TCPAddDocumentRequest *msg);

};

#endif // TCPMESSAGES_H
