#ifndef TCPADDDOCUMENTREQUEST_H
#define TCPADDDOCUMENTREQUEST_H

#include "tcpmessages_global.h"
#include "messageBase.h"

struct TCPMESSAGESSHARED_EXPORT TCPAddDocumentRequest : public messageBase
{
    QByteArray blob;
};

#endif // TCPADDDOCUMENTREQUEST_H
