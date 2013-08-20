#ifndef TCPADDDOCUMENTREQUEST_H
#define TCPADDDOCUMENTREQUEST_H

#include <QObject>
#include "messagebase.h"
#include "tcpmessages_global.h"


class TCPMESSAGESSHARED_EXPORT TCPAddDocumentRequest : public MessageBase
{
    Q_OBJECT
public:
    explicit TCPAddDocumentRequest(QObject *parent = 0);
    virtual ~TCPAddDocumentRequest();
    void setBlob(const QByteArray &blob);
    virtual QByteArray asBlob();
signals:
    
public slots:

private:
    QByteArray m_Blob;
};

#endif // TCPADDDOCUMENTREQUEST_H
