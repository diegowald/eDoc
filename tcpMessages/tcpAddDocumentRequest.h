#ifndef TCPADDDOCUMENTREQUEST_H
#define TCPADDDOCUMENTREQUEST_H

#include <QObject>
#include "messageBase.h"
#include "tcpmessages_global.h"


class TCPMESSAGESSHARED_EXPORT AddDocumentRequest : public MessageBase
{
    Q_OBJECT
public:
    explicit AddDocumentRequest(QObject *parent = 0);
    virtual ~AddDocumentRequest();
    virtual void setBlob(const QByteArray &blob);
signals:
    
public slots:

public:
    QByteArray m_Blob;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<( QDataStream& dataStream, const AddDocumentRequest& message);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>( QDataStream& dataStream, AddDocumentRequest& message); // deprecated: throw( UserException )

#endif // TCPADDDOCUMENTREQUEST_H
