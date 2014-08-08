#include "tcpcommunicator.h"
#include "adddocumentresponse.h"
#include <QDataStream>
#include <QUuid>

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

    connect(&messageParser, SIGNAL(ChunkArrived(TCPChunck&)),
            this, SLOT(ChunkArrived(TCPChunck&)));

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
    IDocID * id = NULL;
    if (!msg.requiresSplit())
    {
        id = m_DocFactory->docEngine()->addDocument(msg.m_Blob);
        AddDocumentResponse resp;
        resp.m_DocID = id->asString();
    }
    else
    {
        id = QUuid::createUuid(); esta mal

    }
    sendData(&resp);
}

void TCPCommunicator::ChunkArrived(TCPChunck &msg)
{
}

void TCPCommunicator::badMessage()
{
}

void TCPCommunicator::sendData(MessageBase *msg)
{
    QByteArray block;
    QDataStream ds(&block, QIODevice::WriteOnly);
    ds << *msg;
    if (m_Socket->isOpen())
    {
        qint64 result = m_Socket->write(block);
        m_logger->logDebug(QString::number(result));
    }
    else
    {
        m_logger->logDebug("Socket is closed.");
    }
}
