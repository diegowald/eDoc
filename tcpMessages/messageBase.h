#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include "tcpmessages_global.h"

struct TCPMESSAGESSHARED_EXPORT messageBase
{
    virtual QByteArray asBlob() = 0;
    int messageSize;
    enum MessageType
    {
        ADD_DOCUMENT
    } messageType;
};

#endif // MESSAGEBASE_H
