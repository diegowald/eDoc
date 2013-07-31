#ifndef IMETADATA_H
#define IMETADATA_H

#include "IUser.h"

struct IMetadata
{
    virtual IUser *ownerUser() = 0;
    virtual IGroup *ownerGroup() = 0;
    virtual bool canReadByUser(IUser *user) = 0;
    virtual bool canModifyByUser(IUser *user) = 0;
    virtual void changeUser(IUser *newUser) = 0;
    virtual void changeGroup(IGroup *newGroup) = 0;
    virtual ~IMetadata() {}
};

#endif // IMETADATA_H
