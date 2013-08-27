#ifndef TCPADDDOCUMENTREQUEST_H
#define TCPADDDOCUMENTREQUEST_H

#include <QObject>
#include "messageBase.h"
#include "tcpmessages_global.h"


class TCPMESSAGESSHARED_EXPORT TCPAddDocumentRequest : public MessageBase
{
    Q_OBJECT
public:
    explicit TCPAddDocumentRequest(QObject *parent = 0);
    virtual ~TCPAddDocumentRequest();
    virtual void setBlob(const QByteArray &blob);
signals:
    
public slots:

public:
    QByteArray m_Blob;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<( QDataStream& dataStream, const TCPAddDocumentRequest& message);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>( QDataStream& dataStream, TCPAddDocumentRequest& message); // deprecated: throw( UserException )

#endif // TCPADDDOCUMENTREQUEST_H
