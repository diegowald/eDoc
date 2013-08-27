#ifndef TCPMESSAGES_H
#define TCPMESSAGES_H

#include "tcpmessages_global.h"
#include "messageBase.h"
#include "tcpAddDocumentRequest.h"
#include <QDataStream>


class TCPMESSAGESSHARED_EXPORT TcpMessages : public QObject
{
    Q_OBJECT
public:
    TcpMessages();
    void parse(QDataStream &dataStream);
signals:
    void addDocumentRequestArrived(TCPAddDocumentRequest &msg);

};

#endif // TCPMESSAGES_H
