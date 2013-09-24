#include "tcpAddDocumentRequest.h"

AddDocumentRequest::AddDocumentRequest(int chunkSize, QObject *parent) :
    MessageBase(ADD_DOCUMENT_REQ, parent)
{
    m_ChunkSize = chunkSize;
}

AddDocumentRequest::~AddDocumentRequest()
{
}

void AddDocumentRequest::setBlob(const QByteArray &blob)
{
    m_Blob = blob;
}

bool AddDocumentRequest::requiresSplit() const
{
    if (m_ChunkSize == 0)
        return false;
    return m_Blob.size() > m_ChunkSize;
}

int AddDocumentRequest::getChunkCount() const
{
    int count = m_Blob.size() / m_ChunkSize;
    if (m_Blob.size() % m_ChunkSize > 0)
        count++;
    return count;
}

TCPChunck *AddDocumentRequest::getChunk(int chunkId)
{
    TCPChunck *ch = new TCPChunck(this);
    int first = chunkId * m_ChunkSize;
    ch->m_Blob = m_Blob.mid(first, m_ChunkSize);
    return ch;
}

QDataStream& operator<<(QDataStream& dataStream, const AddDocumentRequest& message)
{
    dataStream << (*(MessageBase*)(&message))
               << message.getChunkCount();
    if (!message.requiresSplit())
        dataStream << message.m_Blob;
    return dataStream;
}

// Important: this will throw a UserException on error
QDataStream& operator>>(QDataStream& dataStream, AddDocumentRequest& message) // deprecated: throw( UserException )
{
    dataStream >> (*(MessageBase*)(&message));
    int chunkCount = 0;
    dataStream >> chunkCount;
    if (chunkCount < 2)
        dataStream >> message.m_Blob;
    return dataStream;
}
