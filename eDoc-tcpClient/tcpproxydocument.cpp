#include "tcpproxydocument.h"
#include "../tcpMessages/getblobrequest.h"
#include "../tcpMessages/getblobresponse.h"

TCPProxyDocument::TCPProxyDocument(QTcpSocket *Socket, QObject *parent) :
    QObject(parent), tcpSocket(Socket)
{
}

TCPProxyDocument::TCPProxyDocument(QTcpSocket *Socket, IDocID *id, QObject *parent) :
    QObject(parent), m_ID(id), tcpSocket(Socket)
{
}

TCPProxyDocument::~TCPProxyDocument()
{
}

IDocID* TCPProxyDocument::id()
{
    return m_ID;
}

QByteArray TCPProxyDocument::blob()
{
    QByteArray block;
    QDataStream ds(&block, QIODevice::WriteOnly);

    getBlobRequest req(m_ID, this);
    ds << req;
    /*if (!tcpSocket->isOpen())
    {
        tcpSocket->connectToHost(url, port, QTcpSocket::ReadWrite);
        tcpSocket->open(QTcpSocket::ReadWrite);
    }*/
    qint64 result = tcpSocket->write(block);
    //m_Logger->logDebug(QString::number(result));

    while (tcpSocket->bytesAvailable() < 1)
    {
        if (!tcpSocket->waitForReadyRead())
        {
            /*m_Logger->logError(QString("Timeout: %1 %2")
                               .arg(tcpSocket->error())
                               .arg(tcpSocket->errorString()));*/
            return QByteArray();
        }
    }

    QByteArray data = tcpSocket->readAll();
    getBlobResponse b;
    QDataStream ds2(data);
    ds2 >> b;
    return b.blob;
}
