#ifndef TCPCOMMUNICATOR_H
#define TCPCOMMUNICATOR_H

#include <QObject>
#include <QtNetwork>
#include "../eDoc-Factory/edocfactory.h"
#include "../eDoc-Configuration/qobjectlgging.h"
#include "../tcpMessages/tcpmessages.h"
#include "tcpmessages_global.h"


class TCPMESSAGESSHARED_EXPORT TCPCommunicator : public QObject
{
    Q_OBJECT
public:
    explicit TCPCommunicator(QTcpSocket *socket,
                             EDocFactory *f,
                             QObjectLogging *logger,
                             QObject *parent = 0);
    virtual ~TCPCommunicator();

private slots:
    void readyRead();
    void badMessage();
    void addDocumentRequestArrived(AddDocumentRequest &msg);

private:
    void parse();
    void sendData(MessageBase *msg);
private:
    QTcpSocket *m_Socket;
    EDocFactory *m_DocFactory;
    QObjectLogging *m_logger;
    QByteArray readBlock;
    TcpMessages messageParser;
};

#endif // TCPCOMMUNICATOR_H
