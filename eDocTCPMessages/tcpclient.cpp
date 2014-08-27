#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    ipAddress = "";
    port = 0;
    out = NULL;
    timeOut = 15000;

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
}

TcpClient::~TcpClient()
{
    if (tcpSocket->isOpen())
    {
        tcpSocket->abort();
        tcpSocket->close();
    }
    if (out != NULL)
    {
        delete out;
    }
}

void TcpClient::prepareToSend(MessageCodes::CodeNumber code)
{
    if (out != NULL)
    {
        delete out;
    }

    buildingBlob.clear();
    out = new QDataStream(&buildingBlob, QIODevice::WriteOnly);
    out->setVersion(QDataStream::Qt_5_3);

    Header header;
    header.setCommand(code);

    (*out) << header;
}

QByteArray TcpClient::send()
{
    if (!tcpSocket->isOpen())
    {
        tcpSocket->abort();
        tcpSocket->connectToHost(ipAddress, port);
        if (!tcpSocket->waitForConnected(timeOut))
        {
            //error
            return NULL;
        }
    }
    blob.clear();
    QDataStream out(&blob, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);

    QByteArray compressed = qCompress(buildingBlob, 9);
//    out << (buildingBlob.size() + (int)sizeof(int));
//    out << buildingBlob;
    out << (compressed.size() + (int)sizeof(int));
    out << compressed;


    tcpSocket->write(blob);
    tcpSocket->flush();
    tcpSocket->waitForBytesWritten();
    blob.clear();

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_3);


    while (tcpSocket->bytesAvailable() < (int) sizeof(int))
    {
        if (!tcpSocket->waitForReadyRead(timeOut))
        {
            // error
        }
    }
    in >> blockSize;

    while (tcpSocket->bytesAvailable() < blockSize)
    {
        if (!tcpSocket->waitForReadyRead(timeOut) < blockSize)
        {
            // error
        }
    }

    QByteArray response;
    in >> response;
    QByteArray uncompressed = qUncompress(response);
    return uncompressed;
}

void TcpClient::on_error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
        emit error(socketError, QString(tr("The host was not found. Please check the "
                                     "host name and port settings.")));
         break;
     case QAbstractSocket::ConnectionRefusedError:
        emit error(socketError, tr("The connection was refused by the peer. "
                                     "Make sure the fortune server is running, "
                                     "and check that the host name and port "
                                     "settings are correct."));
         break;
     default:
        emit error(socketError, tr("The following error occurred: %1.")
                         .arg(tcpSocket->errorString()));
     }
}

void TcpClient::connected()
{
//    tcpSocket->write(blob);
//    tcpSocket->flush();
//    tcpSocket->waitForBytesWritten();
//    blob.clear();
}
