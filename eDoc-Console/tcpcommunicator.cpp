#include "tcpcommunicator.h"

TCPCommunicator::TCPCommunicator(QTcpSocket *socket, EDocFactory *f, QObjectLogging *logger, QObject *parent) :
    QObject(parent)
{
    m_Socket = socket;
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));

    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readyRead()));

    m_DocFactory = f;
    m_logger = logger;
}

TCPCommunicator::~TCPCommunicator()
{
    if (m_Socket->isOpen())
        m_Socket->disconnectFromHost();
}

void TCPCommunicator::readyRead()
{
    readBlock = m_Socket->readAll();
    parse();
}

void TCPCommunicator::parse()
{
    QDataStream ds(readBlock);
}
