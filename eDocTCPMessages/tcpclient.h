#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QSharedPointer>
#include "../eDocTCPMessages/header.h"
#include "../eDocTCPMessages/messagecodes.h"

class EDOCTCPMESSAGESSHARED_EXPORT TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    virtual ~TcpClient();

protected:
    void process(Header &header);

    void prepareToSend(MessageCodes::CodeNumber code);
    QByteArray send();

signals:
    void error(QAbstractSocket::SocketError socketError, const QString &errorMessage);

private slots:
    void on_error(QAbstractSocket::SocketError socketError);

protected:
    QString ipAddress;
    int port;
    QSharedPointer<QDataStream> out;

private:

    QSharedPointer<QTcpSocket> tcpSocket;

    int blockSize;
    int timeOut;

    QByteArray blob;

    QByteArray buildingBlob;
};

#endif // EDOCTCPCLIENT_H

