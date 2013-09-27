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
    void badMessage();
    void addDocumentRequestArrived(AddDocumentRequest &msg);
    void ChunkArrived(TCPChunck &msg);
};

#endif // TCPMESSAGES_H
