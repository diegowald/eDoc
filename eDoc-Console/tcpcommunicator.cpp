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

    connect(&messageParser, SIGNAL(addDocumentRequestArrived(AddDocumentRequest&)),
            this, SLOT(addDocumentRequestArrived(AddDocumentRequest&)));

    connect(&messageParser, SIGNAL(badMessage()),
            this, SLOT(badMessage()));
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
    messageParser.parse(ds);
}

void TCPCommunicator::addDocumentRequestArrived(AddDocumentRequest &msg)
{
}

void TCPCommunicator::badMessage()
{
}
