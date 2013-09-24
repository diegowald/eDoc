#ifndef TCPADDDOCUMENTREQUEST_H
#define TCPADDDOCUMENTREQUEST_H

#include <QObject>
#include "messageBase.h"
#include "tcpmessages_global.h"
#include "tcpchunck.h"

class TCPMESSAGESSHARED_EXPORT AddDocumentRequest : public MessageBase
{
    Q_OBJECT
public:
    explicit AddDocumentRequest(int chunkSize = 0, QObject *parent = 0);
    virtual ~AddDocumentRequest();
    virtual void setBlob(const QByteArray &blob);
    virtual bool requiresSplit() const;
    virtual TCPChunck *getChunk(int chunkId);
    virtual int getChunkCount() const;

signals:
    
public slots:

public:
    QByteArray m_Blob;
private:
    int m_ChunkSize;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<( QDataStream& dataStream, const AddDocumentRequest& message);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>( QDataStream& dataStream, AddDocumentRequest& message); // deprecated: throw( UserException )

#endif // TCPADDDOCUMENTREQUEST_H
