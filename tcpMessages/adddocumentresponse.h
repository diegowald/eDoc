#ifndef ADDDOCUMENTRESPONSE_H
#define ADDDOCUMENTRESPONSE_H

#include <QObject>
#include "messageBase.h"
#include "tcpmessages_global.h"

class TCPMESSAGESSHARED_EXPORT AddDocumentResponse : public MessageBase
{
    Q_OBJECT
public:
    explicit AddDocumentResponse(QObject *parent = 0);    
signals:
    
public slots:
    
public:
    QString m_DocID;
};

TCPMESSAGESSHARED_EXPORT QDataStream& operator<<(QDataStream& dataStream, const AddDocumentResponse& msg);

// Important: this will throw a UserException on error
TCPMESSAGESSHARED_EXPORT QDataStream& operator>>(QDataStream& dataStream, AddDocumentResponse& msg); // deprecated: throw( UserException )

#endif // ADDDOCUMENTRESPONSE_H
