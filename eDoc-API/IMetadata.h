#ifndef IMETADATA_H
#define IMETADATA_H

#include <QSharedPointer>
#include "IUser.h"

struct IMetadata
{
    virtual IUserPtr ownerUser() = 0;
    virtual IGroupPtr ownerGroup() = 0;
    virtual bool canReadByUser(IUserPtr user) = 0;
    virtual bool canModifyByUser(IUserPtr user) = 0;
    virtual void changeUser(IUserPtr newUser) = 0;
    virtual void changeGroup(IGroupPtr newGroup) = 0;
    virtual ~IMetadata() {}
};

typedef QSharedPointer<IMetadata> IMetadataPtr;

#endif // IMETADATA_H
