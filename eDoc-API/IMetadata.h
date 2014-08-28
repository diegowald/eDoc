#ifndef IMETADATA_H
#define IMETADATA_H

#include "IUser.h"

struct IMetadata
{
    virtual QSharedPointer<IUser> ownerUser() = 0;
    virtual QSharedPointer<IGroup> ownerGroup() = 0;
    virtual bool canReadByUser(QSharedPointer<IUser> user) = 0;
    virtual bool canModifyByUser(QSharedPointer<IUser> user) = 0;
    virtual void changeUser(QSharedPointer<IUser> newUser) = 0;
    virtual void changeGroup(QSharedPointer<IGroup> newGroup) = 0;
    virtual ~IMetadata() {}
};

#endif // IMETADATA_H
